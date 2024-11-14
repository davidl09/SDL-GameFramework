#include "collider.h"
#include <cmath>
#include <algorithm>

Collider::Collider(Type type, float width, float height)
    : type(type)
    , width(width)
    , height(height)
{
}

Collider::Collider(Type type, const std::vector<Vector2D>& points)
    : type(type)
    , points(points)
{
    // Calculate bounding box for the polygon
    if (!points.empty()) {
        auto [minX, maxX] = std::minmax_element(points.begin(), points.end(),
            [](const Vector2D& a, const Vector2D& b) { return a.x < b.x; });
        auto [minY, maxY] = std::minmax_element(points.begin(), points.end(),
            [](const Vector2D& a, const Vector2D& b) { return a.y < b.y; });

        width = maxX->x - minX->x;
        height = maxY->y - minY->y;
    }
}

bool Collider::CheckCollision(const Collider& other, const Transform& thisTransform,
                            const Transform& otherTransform) const {
    if (type == Type::Box && other.type == Type::Box) {
        return CheckBoxCollision(other, thisTransform, otherTransform);
    }
    else if (type == Type::Circle && other.type == Type::Circle) {
        return CheckCircleCollision(other, thisTransform, otherTransform);
    }
    else if (type == Type::Polygon && other.type == Type::Polygon) {
        return CheckPolygonCollision(other, thisTransform, otherTransform);
    }
    // For mixed collisions, use box collision as a simple approximation
    return CheckBoxCollision(other, thisTransform, otherTransform);
}

SDL_Rect Collider::GetBounds(const Transform& transform) const {
    SDL_Rect bounds;
    bounds.x = static_cast<int>(transform.position.x - (width * transform.scale.x) / 2);
    bounds.y = static_cast<int>(transform.position.y - (height * transform.scale.y) / 2);
    bounds.w = static_cast<int>(width * transform.scale.x);
    bounds.h = static_cast<int>(height * transform.scale.y);
    return bounds;
}

const std::vector<Vector2D>& Collider::GetPoints() const {
    return points;
}

void Collider::RenderDebug(SDL_Renderer* renderer, const Transform& transform) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    switch (type) {
        case Type::Box: {
            SDL_Rect bounds = GetBounds(transform);
            SDL_RenderDrawRect(renderer, &bounds);
            break;
        }
        case Type::Circle: {
            // Approximate circle with lines
            const int segments = 32;
            const float radius = width * transform.scale.x / 2;
            float angleStep = 2 * M_PI / segments;

            for (int i = 0; i < segments; ++i) {
                float angle1 = i * angleStep;
                float angle2 = (i + 1) * angleStep;

                int x1 = static_cast<int>(transform.position.x + radius * std::cos(angle1));
                int y1 = static_cast<int>(transform.position.y + radius * std::sin(angle1));
                int x2 = static_cast<int>(transform.position.x + radius * std::cos(angle2));
                int y2 = static_cast<int>(transform.position.y + radius * std::sin(angle2));

                SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
            }
            break;
        }
        case Type::Polygon: {
            auto transformedPoints = GetTransformedPoints(transform);
            for (size_t i = 0; i < transformedPoints.size(); ++i) {
                size_t j = (i + 1) % transformedPoints.size();
                SDL_RenderDrawLine(renderer,
                    static_cast<int>(transformedPoints[i].x),
                    static_cast<int>(transformedPoints[i].y),
                    static_cast<int>(transformedPoints[j].x),
                    static_cast<int>(transformedPoints[j].y));
            }
            break;
        }
    }
}

bool Collider::CheckBoxCollision(const Collider& other, const Transform& thisTransform,
                               const Transform& otherTransform) const {
    SDL_Rect rectA = GetBounds(thisTransform);
    SDL_Rect rectB = other.GetBounds(otherTransform);

    return SDL_HasIntersection(&rectA, &rectB) == SDL_TRUE;
}

bool Collider::CheckCircleCollision(const Collider& other, const Transform& thisTransform,
                                  const Transform& otherTransform) const {
    float radiusA = (width * thisTransform.scale.x) / 2;
    float radiusB = (other.width * otherTransform.scale.x) / 2;

    float dx = thisTransform.position.x - otherTransform.position.x;
    float dy = thisTransform.position.y - otherTransform.position.y;
    float distance = std::sqrt(dx * dx + dy * dy);

    return distance < (radiusA + radiusB);
}

bool Collider::CheckPolygonCollision(const Collider& other, const Transform& thisTransform,
                                   const Transform& otherTransform) const {
    auto points1 = GetTransformedPoints(thisTransform);
    auto points2 = other.GetTransformedPoints(otherTransform);

    // Using Separating Axis Theorem (SAT)
    return SATOverlap(points1, points2) && SATOverlap(points2, points1);
}

std::vector<Vector2D> Collider::GetTransformedPoints(const Transform& transform) const {
    std::vector<Vector2D> transformedPoints;
    transformedPoints.reserve(points.size());

    float radians = transform.rotation * M_PI / 180.0f;
    float cos_r = std::cos(radians);
    float sin_r = std::sin(radians);

    for (const auto& point : points) {
        // Scale
        Vector2D scaled(point.x * transform.scale.x, point.y * transform.scale.y);

        // Rotate
        Vector2D rotated(
            scaled.x * cos_r - scaled.y * sin_r,
            scaled.x * sin_r + scaled.y * cos_r
        );

        // Translate
        transformedPoints.emplace_back(
            rotated.x + transform.position.x,
            rotated.y + transform.position.y
        );
    }

    return transformedPoints;
}

bool Collider::SATOverlap(const std::vector<Vector2D>& points1,
                         const std::vector<Vector2D>& points2) const {
    for (size_t i = 0; i < points1.size(); ++i) {
        size_t j = (i + 1) % points1.size();

        // Get the normal to this edge
        Vector2D edge = Vector2D(
            points1[j].x - points1[i].x,
            points1[j].y - points1[i].y
        );
        Vector2D normal(-edge.y, edge.x);

        // Project both shapes onto the normal
        float min1 = std::numeric_limits<float>::max();
        float max1 = std::numeric_limits<float>::lowest();
        float min2 = std::numeric_limits<float>::max();
        float max2 = std::numeric_limits<float>::lowest();

        // Project polygon1
        for (const auto& point : points1) {
            float proj = point.x * normal.x + point.y * normal.y;
            min1 = std::min(min1, proj);
            max1 = std::max(max1, proj);
        }

        // Project polygon2
        for (const auto& point : points2) {
            float proj = point.x * normal.x + point.y * normal.y;
            min2 = std::min(min2, proj);
            max2 = std::max(max2, proj);
        }

        // Check for separation
        if (max1 < min2 || max2 < min1) {
            return false;
        }
    }

    return true;
}