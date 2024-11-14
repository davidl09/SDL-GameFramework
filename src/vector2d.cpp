#include "Vector2D.h"

Vector2D::Vector2D(float x, float y) : x(x), y(y) {}

Vector2D& Vector2D::Add(const Vector2D& vec) {
    x += vec.x;
    y += vec.y;
    return *this;
}

Vector2D& Vector2D::Subtract(const Vector2D& vec) {
    x -= vec.x;
    y -= vec.y;
    return *this;
}

Vector2D& Vector2D::Multiply(const float& scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
}

Vector2D& Vector2D::Divide(const float& scalar) {
    if (scalar != 0.0f) {
        x /= scalar;
        y /= scalar;
    }
    return *this;
}

float Vector2D::Magnitude() const {
    return std::sqrt(x * x + y * y);
}

Vector2D Vector2D::Normalized() const {
    float mag = Magnitude();
    if (mag != 0.0f) {
        return Vector2D(x / mag, y / mag);
    }
    return *this;
}

Vector2D& Vector2D::operator+=(const Vector2D& vec) {
    return Add(vec);
}

Vector2D& Vector2D::operator-=(const Vector2D& vec) {
    return Subtract(vec);
}

Vector2D& Vector2D::operator*=(const float& scalar) {
    return Multiply(scalar);
}

Vector2D& Vector2D::operator/=(const float& scalar) {
    return Divide(scalar);
}
