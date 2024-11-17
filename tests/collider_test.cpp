#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "collider.h"

class ColliderTest : public ::testing::Test {
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

// Box Collider Tests
TEST_F(ColliderTest, BoxColliderConstruction) {
    Collider boxCollider(Collider::Type::Box, 100.0f, 50.0f);
    EXPECT_EQ(boxCollider.GetType(), Collider::Type::Box);

    SDL_Rect bounds = boxCollider.GetBounds(Transform());
    EXPECT_EQ(bounds.w, 100);
    EXPECT_EQ(bounds.h, 50);
}

TEST_F(ColliderTest, BoxColliderIntersection) {
    Collider box1(Collider::Type::Box, 100.0f, 100.0f);
    Collider box2(Collider::Type::Box, 100.0f, 100.0f);

    Transform t1, t2;

    // Test overlapping boxes
    t1.position = Vector2D(0, 0);
    t2.position = Vector2D(50, 50);
    EXPECT_TRUE(box1.CheckCollision(box2, t1, t2));

    // Test non-overlapping boxes - move box2 completely outside box1's bounds
    t2.position = Vector2D(150, 150);  // Changed from 200,200 to ensure no overlap
    EXPECT_FALSE(box1.CheckCollision(box2, t1, t2));
}

TEST_F(ColliderTest, BoxColliderWithScale) {
    Collider box1(Collider::Type::Box, 100.0f, 100.0f);
    Collider box2(Collider::Type::Box, 100.0f, 100.0f);

    Transform t1, t2;
    t1.scale = Vector2D(0.5f, 0.5f);  // Box1 is now effectively 50x50

    // Place box2 just outside box1's scaled bounds
    t1.position = Vector2D(0, 0);
    t2.position = Vector2D(75, 0);  // Should be no collision (50/2 + 100/2 < 75)
    EXPECT_FALSE(box1.CheckCollision(box2, t1, t2));

    // Move box2 to overlap with box1
    t2.position = Vector2D(25, 0);
    EXPECT_TRUE(box1.CheckCollision(box2, t1, t2));
}

// Circle Collider Tests
TEST_F(ColliderTest, CircleColliderIntersection) {
    Collider circle1(Collider::Type::Circle, 100.0f, 100.0f);
    Collider circle2(Collider::Type::Circle, 100.0f, 100.0f);

    Transform t1, t2;

    // Test overlapping circles
    t1.position = Vector2D(0, 0);
    t2.position = Vector2D(50, 0);  // Distance = 50, radius sum = 100
    EXPECT_TRUE(circle1.CheckCollision(circle2, t1, t2));

    // Test non-overlapping circles
    t2.position = Vector2D(150, 0);  // Distance = 150, radius sum = 100
    EXPECT_FALSE(circle1.CheckCollision(circle2, t1, t2));
}

// Polygon Collider Tests
TEST_F(ColliderTest, PolygonColliderConstruction) {
    std::vector<Vector2D> points = {
        Vector2D(-50, -50),
        Vector2D(50, -50),
        Vector2D(50, 50),
        Vector2D(-50, 50)
    };

    Collider polyCollider(Collider::Type::Polygon, points);
    EXPECT_EQ(polyCollider.GetType(), Collider::Type::Polygon);

    const auto& colliderPoints = polyCollider.GetPoints();
    EXPECT_EQ(colliderPoints.size(), 4);
    for (size_t i = 0; i < points.size(); ++i) {
        EXPECT_TRUE(VectorsEqual(colliderPoints[i], points[i]));
    }
}

TEST_F(ColliderTest, PolygonColliderIntersection) {
    std::vector<Vector2D> points = {
        Vector2D(-50, -50),
        Vector2D(50, -50),
        Vector2D(50, 50),
        Vector2D(-50, 50)
    };

    Collider poly1(Collider::Type::Polygon, points);
    Collider poly2(Collider::Type::Polygon, points);

    Transform t1, t2;

    // Test overlapping polygons
    t1.position = Vector2D(0, 0);
    t2.position = Vector2D(25, 25);
    EXPECT_TRUE(poly1.CheckCollision(poly2, t1, t2));

    // Test non-overlapping polygons
    t2.position = Vector2D(150, 150);
    EXPECT_FALSE(poly1.CheckCollision(poly2, t1, t2));
}