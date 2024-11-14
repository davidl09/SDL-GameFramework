#pragma once
#include <SDL2/SDL.h>
#include "vector2d.h"

class Mouse {
public:
    static Mouse& Instance();

    // Delete copy constructor and assignment operator
    Mouse(const Mouse&) = delete;
    Mouse& operator=(const Mouse&) = delete;

    // Mouse state
    [[nodiscard]] Vector2D GetPosition() const;
    [[nodiscard]] Vector2D GetWorldPosition() const;  // In case you add camera support later
    [[nodiscard]] Vector2D GetRelativeMotion() const;
    bool IsButtonPressed(int button) const;
    bool IsButtonJustPressed(int button) const;
    bool IsButtonJustReleased(int button) const;

    // Mouse button constants for easier use
    static constexpr int LEFT = SDL_BUTTON_LEFT;
    static constexpr int RIGHT = SDL_BUTTON_RIGHT;
    static constexpr int MIDDLE = SDL_BUTTON_MIDDLE;

    // Should be called each frame to update button states
    void Update();

private:
    Mouse() = default;

    // Current and previous frame button states
    Uint32 currentButtonState = 0;
    Uint32 previousButtonState = 0;
    Vector2D position;
    Vector2D relativeMotion;
};