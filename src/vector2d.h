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

    // Comparison operators
    bool operator==(const Vector2D& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Vector2D& other) const {
        return !(*this == other);
    }

    // Arithmetic operators
    Vector2D operator+(const Vector2D& other) const {
        return Vector2D(x + other.x, y + other.y);
    }

    Vector2D operator-(const Vector2D& other) const {
        return Vector2D(x - other.x, y - other.y);
    }

    // Scalar operations
    Vector2D operator*(float scalar) const {
        return Vector2D(x * scalar, y * scalar);
    }

    Vector2D operator/(float scalar) const {
        return Vector2D(x / scalar, y / scalar);
    }

    // Friend operator for scalar * vector
    friend Vector2D operator*(float scalar, const Vector2D& vec) {
        return vec * scalar;
    }
};
