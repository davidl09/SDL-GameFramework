#pragma once
#include "vector2d.h"

class Transform {
public:
    Vector2D position;
    Vector2D scale;
    float rotation;
    
    Transform(const Vector2D& pos = Vector2D(), 
             const Vector2D& scale = Vector2D(1.0f, 1.0f),
             float rotation = 0.0f);

    // Utility functions for transformation
    void Translate(const Vector2D& translation);
    void Rotate(float angle);
    void SetScale(const Vector2D& newScale);
    
    // Get forward and right vectors based on rotation
    Vector2D GetForward() const;
    Vector2D GetRight() const;
    
    // Get position
    const Vector2D& GetPosition() const { return position; }
};
