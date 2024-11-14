#pragma once
#include <cmath>

class Vector2D {
public:
    float x, y;
    
    Vector2D(float x = 0.0f, float y = 0.0f);
    
    Vector2D& Add(const Vector2D& vec);
    Vector2D& Subtract(const Vector2D& vec);
    Vector2D& Multiply(const float& scalar);
    Vector2D& Divide(const float& scalar);
    
    float Magnitude() const;
    Vector2D Normalized() const;
    
    Vector2D& operator+=(const Vector2D& vec);
    Vector2D& operator-=(const Vector2D& vec);
    Vector2D& operator*=(const float& scalar);
    Vector2D& operator/=(const float& scalar);
};
