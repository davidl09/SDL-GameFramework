#include <gtest/gtest.h>
#include "vector2d.h"

class Vector2DTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Common setup for each test
    }

    // Helper function for floating point comparison
    bool NearlyEqual(float a, float b, float epsilon = 0.0001f) {
        return std::abs(a - b) <= epsilon;
    }

    bool VectorsEqual(const Vector2D& a, const Vector2D& b, float epsilon = 0.0001f) {
        return NearlyEqual(a.x, b.x, epsilon) && NearlyEqual(a.y, b.y, epsilon);
    }
};

TEST_F(Vector2DTest, DefaultConstructor) {
    Vector2D vec;
    EXPECT_FLOAT_EQ(vec.x, 0.0f);
    EXPECT_FLOAT_EQ(vec.y, 0.0f);
}

TEST_F(Vector2DTest, ParameterizedConstructor) {
    Vector2D vec(2.0f, 3.0f);
    EXPECT_FLOAT_EQ(vec.x, 2.0f);
    EXPECT_FLOAT_EQ(vec.y, 3.0f);
}

TEST_F(Vector2DTest, Addition) {
    Vector2D vec1(1.0f, 2.0f);
    Vector2D vec2(3.0f, 4.0f);

    vec1 += vec2;
    EXPECT_FLOAT_EQ(vec1.x, 4.0f);
    EXPECT_FLOAT_EQ(vec1.y, 6.0f);
}

TEST_F(Vector2DTest, Subtraction) {
    Vector2D vec1(5.0f, 7.0f);
    Vector2D vec2(2.0f, 3.0f);

    vec1 -= vec2;
    EXPECT_FLOAT_EQ(vec1.x, 3.0f);
    EXPECT_FLOAT_EQ(vec1.y, 4.0f);
}

TEST_F(Vector2DTest, ScalarMultiplication) {
    Vector2D vec(2.0f, 3.0f);
    vec *= 2.0f;

    EXPECT_FLOAT_EQ(vec.x, 4.0f);
    EXPECT_FLOAT_EQ(vec.y, 6.0f);
}

TEST_F(Vector2DTest, ScalarDivision) {
    Vector2D vec(4.0f, 6.0f);
    vec /= 2.0f;

    EXPECT_FLOAT_EQ(vec.x, 2.0f);
    EXPECT_FLOAT_EQ(vec.y, 3.0f);
}

TEST_F(Vector2DTest, Magnitude) {
    Vector2D vec(3.0f, 4.0f);
    EXPECT_FLOAT_EQ(vec.Magnitude(), 5.0f);
}

TEST_F(Vector2DTest, Normalization) {
    Vector2D vec(3.0f, 4.0f);
    Vector2D normalized = vec.Normalized();

    EXPECT_FLOAT_EQ(normalized.x, 0.6f);
    EXPECT_FLOAT_EQ(normalized.y, 0.8f);
    EXPECT_NEAR(normalized.Magnitude(), 1.0f, 0.0001f);
}

TEST_F(Vector2DTest, DivideByZero) {
    Vector2D vec(4.0f, 6.0f);
    vec /= 0.0f;

    // Vector should remain unchanged when dividing by zero
    EXPECT_FLOAT_EQ(vec.x, 4.0f);
    EXPECT_FLOAT_EQ(vec.y, 6.0f);
}

TEST_F(Vector2DTest, ZeroVectorNormalization) {
    Vector2D vec(0.0f, 0.0f);
    Vector2D normalized = vec.Normalized();

    // A zero vector normalized should still be zero
    EXPECT_FLOAT_EQ(normalized.x, 0.0f);
    EXPECT_FLOAT_EQ(normalized.y, 0.0f);
}