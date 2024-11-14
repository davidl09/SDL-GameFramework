#include "transform.h"
#include <cmath>

Transform::Transform(const Vector2D& pos, const Vector2D& scale, float rotation)
    : position(pos), scale(scale), rotation(rotation) {}

void Transform::Translate(const Vector2D& translation) {
    position += translation;
}

void Transform::Rotate(float angle) {
    rotation += angle;
    // Normalize rotation to 0-360 degrees
    while (rotation >= 360.0f) rotation -= 360.0f;
    while (rotation < 0.0f) rotation += 360.0f;
}

void Transform::SetScale(const Vector2D& newScale) {
    scale = newScale;
}

Vector2D Transform::GetForward() const {
    float radians = rotation * (M_PI / 180.0f);
    return Vector2D(std::cos(radians), std::sin(radians));
}

Vector2D Transform::GetRight() const {
    float radians = (rotation + 90.0f) * (M_PI / 180.0f);
    return Vector2D(std::cos(radians), std::sin(radians));
}
