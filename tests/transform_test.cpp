#include <gtest/gtest.h>
#include "transform.h"
#include <cmath>

class TransformTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Common setup for each test
    }

    bool NearlyEqual(float a, float b, float epsilon = 0.0001f) {
        return std::abs(a - b) <= epsilon;
    }

    bool VectorsEqual(const Vector2D& a, const Vector2D& b, float epsilon = 0.0001f) {
        return NearlyEqual(a.x, b.x, epsilon) && NearlyEqual(a.y, b.y, epsilon);
    }
};

TEST_F(TransformTest, DefaultConstructor) {
    Transform transform;
    EXPECT_TRUE(VectorsEqual(transform.position, Vector2D(0.0f, 0.0f)));
    EXPECT_TRUE(VectorsEqual(transform.scale, Vector2D(1.0f, 1.0f)));
    EXPECT_FLOAT_EQ(transform.rotation, 0.0f);
}

TEST_F(TransformTest, ParameterizedConstructor) {
    Vector2D pos(2.0f, 3.0f);
    Vector2D scale(2.0f, 2.0f);
    float rotation = 45.0f;

    Transform transform(pos, scale, rotation);
    EXPECT_TRUE(VectorsEqual(transform.position, pos));
    EXPECT_TRUE(VectorsEqual(transform.scale, scale));
    EXPECT_FLOAT_EQ(transform.rotation, rotation);
}

TEST_F(TransformTest, Translation) {
    Transform transform;
    transform.Translate(Vector2D(2.0f, 3.0f));

    EXPECT_TRUE(VectorsEqual(transform.position, Vector2D(2.0f, 3.0f)));
}

TEST_F(TransformTest, Rotation) {
    Transform transform;
    transform.Rotate(45.0f);
    EXPECT_FLOAT_EQ(transform.rotation, 45.0f);

    // Test rotation wraparound
    transform.Rotate(360.0f);
    EXPECT_FLOAT_EQ(transform.rotation, 45.0f);

    transform.Rotate(-405.0f); // -405 + 360 = -45
    EXPECT_FLOAT_EQ(transform.rotation, 0.0f);
}

TEST_F(TransformTest, Scale) {
    Transform transform;
    transform.SetScale(Vector2D(2.0f, 3.0f));

    EXPECT_TRUE(VectorsEqual(transform.scale, Vector2D(2.0f, 3.0f)));
}

TEST_F(TransformTest, GetForward) {
    Transform transform;

    // Forward at 0 degrees should be (1, 0)
    Vector2D forward = transform.GetForward();
    EXPECT_TRUE(VectorsEqual(forward, Vector2D(1.0f, 0.0f)));

    // Forward at 90 degrees should be (0, 1)
    transform.rotation = 90.0f;
    forward = transform.GetForward();
    EXPECT_TRUE(VectorsEqual(forward, Vector2D(0.0f, 1.0f), 0.0001f));

    // Forward at 180 degrees should be (-1, 0)
    transform.rotation = 180.0f;
    forward = transform.GetForward();
    EXPECT_TRUE(VectorsEqual(forward, Vector2D(-1.0f, 0.0f), 0.0001f));
}

TEST_F(TransformTest, GetRight) {
    Transform transform;

    // Right at 0 degrees should be (0, 1)
    Vector2D right = transform.GetRight();
    EXPECT_TRUE(VectorsEqual(right, Vector2D(0.0f, 1.0f), 0.0001f));

    // Right at 90 degrees should be (-1, 0)
    transform.rotation = 90.0f;
    right = transform.GetRight();
    EXPECT_TRUE(VectorsEqual(right, Vector2D(-1.0f, 0.0f), 0.0001f));
}

TEST_F(TransformTest, ComplexTransformation) {
    Transform transform;

    // Apply multiple transformations
    transform.Translate(Vector2D(2.0f, 3.0f));
    transform.Rotate(45.0f);
    transform.SetScale(Vector2D(2.0f, 2.0f));

    EXPECT_TRUE(VectorsEqual(transform.position, Vector2D(2.0f, 3.0f)));
    EXPECT_FLOAT_EQ(transform.rotation, 45.0f);
    EXPECT_TRUE(VectorsEqual(transform.scale, Vector2D(2.0f, 2.0f)));

    // Check forward vector after transformations
    float sqrt2_2 = std::sqrt(2.0f) / 2.0f;
    Vector2D expectedForward(sqrt2_2, sqrt2_2);
    EXPECT_TRUE(VectorsEqual(transform.GetForward(), expectedForward, 0.0001f));
}