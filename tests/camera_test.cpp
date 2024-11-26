#include <gtest/gtest.h>
#include "camera.h"

class CameraTest : public ::testing::Test {
protected:
    void SetUp() override {
        SDL_Rect viewport = {0, 0, 800, 600};
        Camera::Instance().Initialize(viewport);
    }

    void TearDown() override {
        // Reset camera to default state
        SDL_Rect viewport = {0, 0, 800, 600};
        Camera::Instance().Initialize(viewport);
    }
};

TEST_F(CameraTest, InitialState) {
    auto& camera = Camera::Instance();
    EXPECT_EQ(camera.GetPosition(), Vector2D(0, 0));
    EXPECT_FLOAT_EQ(camera.GetZoom(), 1.0f);
    EXPECT_FLOAT_EQ(camera.GetRotation(), 0.0f);
}

TEST_F(CameraTest, SetPosition) {
    auto& camera = Camera::Instance();
    Vector2D newPos(100, 200);
    camera.SetPosition(newPos);
    EXPECT_EQ(camera.GetPosition(), newPos);
}

TEST_F(CameraTest, SetZoom) {
    auto& camera = Camera::Instance();
    
    camera.SetZoom(2.0f);
    EXPECT_FLOAT_EQ(camera.GetZoom(), 2.0f);
    
    // Test zoom limits
    camera.SetZoom(0.05f);
    EXPECT_FLOAT_EQ(camera.GetZoom(), 0.1f); // Should clamp to minimum
    
    camera.SetZoom(11.0f);
    EXPECT_FLOAT_EQ(camera.GetZoom(), 10.0f); // Should clamp to maximum
}

TEST_F(CameraTest, SetRotation) {
    auto& camera = Camera::Instance();
    float rotation = 45.0f;
    camera.SetRotation(rotation);
    EXPECT_FLOAT_EQ(camera.GetRotation(), rotation);
}

TEST_F(CameraTest, WorldToScreen) {
    auto& camera = Camera::Instance();
    camera.SetPosition(Vector2D(100, 100));
    camera.SetZoom(2.0f);
    
    Vector2D worldPos(150, 150);
    Vector2D screenPos = camera.WorldToScreen(worldPos);
    
    // With camera at (100,100) and zoom 2x, point at (150,150) should be offset from center
    Vector2D expected(
        400 + (150 - 100) * 2, // center.x + (world.x - camera.x) * zoom
        300 + (150 - 100) * 2  // center.y + (world.y - camera.y) * zoom
    );
    
    EXPECT_NEAR(screenPos.x, expected.x, 0.1f);
    EXPECT_NEAR(screenPos.y, expected.y, 0.1f);
}

TEST_F(CameraTest, ScreenToWorld) {
    auto& camera = Camera::Instance();
    camera.SetPosition(Vector2D(100, 100));
    camera.SetZoom(2.0f);
    
    Vector2D screenPos(500, 400);
    Vector2D worldPos = camera.ScreenToWorld(screenPos);
    
    // Convert back from screen to world coordinates
    Vector2D expected(
        100 + (500 - 400) / 2.0f, // camera.x + (screen.x - center.x) / zoom
        100 + (400 - 300) / 2.0f  // camera.y + (screen.y - center.y) / zoom
    );
    
    EXPECT_NEAR(worldPos.x, expected.x, 0.1f);
    EXPECT_NEAR(worldPos.y, expected.y, 0.1f);
}

TEST_F(CameraTest, ViewRect) {
    auto& camera = Camera::Instance();
    camera.SetPosition(Vector2D(100, 100));
    camera.SetZoom(2.0f);
    
    SDL_Rect view = camera.GetViewRect();
    
    // At 2x zoom, the view should be half the size of the viewport
    EXPECT_EQ(view.w, 400); // 800/2
    EXPECT_EQ(view.h, 300); // 600/2
    
    // The view should be centered on the camera position
    EXPECT_EQ(view.x, -100); // camera.x - view.w/2
    EXPECT_EQ(view.y, -50);  // camera.y - view.h/2
}

TEST_F(CameraTest, IsVisible) {
    auto& camera = Camera::Instance();
    camera.SetPosition(Vector2D(400, 300));
    
    SDL_Rect objectRect = {350, 250, 100, 100};
    EXPECT_TRUE(camera.IsVisible(objectRect));
    
    objectRect = {1000, 1000, 100, 100};
    EXPECT_FALSE(camera.IsVisible(objectRect));
}

TEST_F(CameraTest, Shake) {
    auto& camera = Camera::Instance();
    Vector2D originalPos(100, 100);
    camera.SetPosition(originalPos);
    
    camera.Shake(0.5f, 10.0f);
    camera.Update(0.1f);
    
    // Position should be offset by shake
    Vector2D shakeOffset = camera.GetShakeOffset();
    EXPECT_NE(shakeOffset.x, 0.0f);
    EXPECT_NE(shakeOffset.y, 0.0f);
    
    // After shake duration
    camera.Update(0.5f);
    shakeOffset = camera.GetShakeOffset();
    EXPECT_FLOAT_EQ(shakeOffset.x, 0.0f);
    EXPECT_FLOAT_EQ(shakeOffset.y, 0.0f);
}
