#pragma once
#include <SDL2/SDL.h>
#include "vector2d.h"
#include "gameobject.h"
#include "game.h"

/**
 * @brief Camera system for controlling view and viewport
 */
class Camera {
public:
    static Camera& Instance() {
        static Camera instance;
        return instance;
    }

    Camera(const Camera&) = delete;
    Camera& operator=(const Camera&) = delete;

    /**
     * @brief Initialize the camera
     * @param viewport Initial viewport size
     */
    void Initialize(const SDL_Rect& viewport) {
        this->viewport = viewport;
        position = Vector2D(viewport.x, viewport.y);
        zoom = 1.0f;
        rotation = 0.0f;
    }

    /**
     * @brief Set the camera's position
     * @param pos New position in world coordinates
     */
    void SetPosition(const Vector2D& pos) {
        position = pos;
    }

    /**
     * @brief Set the camera's zoom level
     * @param newZoom New zoom level (1.0 = normal)
     */
    void SetZoom(float newZoom) {
        zoom = newZoom;
        if (zoom < 0.1f) zoom = 0.1f;
        if (zoom > 10.0f) zoom = 10.0f;
    }

    /**
     * @brief Set the camera's rotation
     * @param degrees Rotation in degrees
     */
    void SetRotation(float degrees) {
        rotation = degrees;
    }

    /**
     * @brief Follow a target game object
     * @param target Target to follow
     * @param smoothing Smoothing factor (0-1, 0 = instant)
     */
    void FollowTarget(GameObject* target, float smoothing = 0.1f) {
        if (!target) return;

        Vector2D targetPos = target->GetTransform().GetPosition();
        Vector2D delta = targetPos - position;
        position += delta * smoothing;
    }

    /**
     * @brief Convert world coordinates to screen coordinates
     * @param worldPos Position in world space
     * @return Position in screen space
     */
    Vector2D WorldToScreen(const Vector2D& worldPos) const {
        // Translate relative to camera
        Vector2D relativePos = worldPos - position;
        
        // Apply zoom
        relativePos *= zoom;
        
        // Apply rotation
        float rad = rotation * M_PI / 180.0f;
        float cos_r = cos(rad);
        float sin_r = sin(rad);
        float x = relativePos.x * cos_r - relativePos.y * sin_r;
        float y = relativePos.x * sin_r + relativePos.y * cos_r;
        
        // Center on screen
        return Vector2D(
            viewport.x + viewport.w / 2.0f + x,
            viewport.y + viewport.h / 2.0f + y
        );
    }

    /**
     * @brief Convert screen coordinates to world coordinates
     * @param screenPos Position in screen space
     * @return Position in world space
     */
    Vector2D ScreenToWorld(const Vector2D& screenPos) const {
        // Center relative to viewport
        Vector2D centered(
            screenPos.x - (viewport.x + viewport.w / 2.0f),
            screenPos.y - (viewport.y + viewport.h / 2.0f)
        );
        
        // Apply inverse rotation
        float rad = -rotation * M_PI / 180.0f;
        float cos_r = cos(rad);
        float sin_r = sin(rad);
        float x = centered.x * cos_r - centered.y * sin_r;
        float y = centered.x * sin_r + centered.y * cos_r;
        
        // Apply inverse zoom and translate back to world space
        return Vector2D(x, y) / zoom + position;
    }

    /**
     * @brief Get the camera's view matrix for rendering
     * @return SDL_Rect representing the view
     */
    SDL_Rect GetViewRect() const {
        return {
            static_cast<int>(position.x - viewport.w / (2.0f * zoom)),
            static_cast<int>(position.y - viewport.h / (2.0f * zoom)),
            static_cast<int>(viewport.w / zoom),
            static_cast<int>(viewport.h / zoom)
        };
    }

    /**
     * @brief Check if a rectangle is visible in the camera's view
     * @param rect Rectangle in world coordinates
     * @return true if visible
     */
    bool IsVisible(const SDL_Rect& rect) const {
        SDL_Rect view = GetViewRect();
        return SDL_HasIntersection(&view, &rect) == SDL_TRUE;
    }

    /**
     * @brief Shake the camera
     * @param duration Shake duration in seconds
     * @param intensity Shake intensity
     */
    void Shake(float duration, float intensity) {
        shakeTime = duration;
        shakeIntensity = intensity;
        isShaking = true;
    }

    /**
     * @brief Update the camera
     * @param deltaTime Time since last update
     */
    void Update(float deltaTime) {
        if (isShaking) {
            shakeTime -= deltaTime;
            if (shakeTime <= 0) {
                isShaking = false;
                shakeOffset = Vector2D();
            } else {
                float randomAngle = static_cast<float>(rand()) / RAND_MAX * M_PI * 2;
                shakeOffset.x = cos(randomAngle) * shakeIntensity;
                shakeOffset.y = sin(randomAngle) * shakeIntensity;
            }
        }
    }

    // Getters
    const Vector2D& GetPosition() const { return position; }
    float GetZoom() const { return zoom; }
    float GetRotation() const { return rotation; }
    const SDL_Rect& GetViewport() const { return viewport; }
    const Vector2D& GetShakeOffset() const { return shakeOffset; }

private:
    Camera() : zoom(1.0f), rotation(0.0f), isShaking(false), 
               shakeTime(0.0f), shakeIntensity(0.0f) {}

    SDL_Rect viewport;      // Current viewport rectangle
    Vector2D position;      // Camera position in world coordinates
    float zoom;            // Camera zoom level
    float rotation;        // Camera rotation in degrees

    // Camera shake properties
    bool isShaking;
    float shakeTime;
    float shakeIntensity;
    Vector2D shakeOffset;
};
