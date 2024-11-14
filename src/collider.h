#pragma once
#include <SDL2/SDL.h>
#include "transform.h"
#include <vector>

class Collider {
public:
    enum class Type {
        Box,
        Circle,
        Polygon
    };
    
    Collider(Type type, float width, float height);
    Collider(Type type, const std::vector<Vector2D>& points); // For polygon collider
    
    bool CheckCollision(const Collider& other, const Transform& thisTransform, 
                       const Transform& otherTransform) const;
    
    SDL_Rect GetBounds(const Transform& transform) const;
    const std::vector<Vector2D>& GetPoints() const;
    Type GetType() const { return type; }
    
    // Debug rendering
    void RenderDebug(SDL_Renderer* renderer, const Transform& transform);
    
private:
    Type type;
    float width;
    float height;
    std::vector<Vector2D> points; // For polygon collider
    
    bool CheckBoxCollision(const Collider& other, const Transform& thisTransform,
                          const Transform& otherTransform) const;
    bool CheckCircleCollision(const Collider& other, const Transform& thisTransform,
                            const Transform& otherTransform) const;
    bool CheckPolygonCollision(const Collider& other, const Transform& thisTransform,
                             const Transform& otherTransform) const;
                             
    // Separating Axis Theorem (SAT) helper functions
    std::vector<Vector2D> GetTransformedPoints(const Transform& transform) const;
    bool SATOverlap(const std::vector<Vector2D>& points1, 
                   const std::vector<Vector2D>& points2) const;
};
