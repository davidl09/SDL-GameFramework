#include "mouse.h"

Mouse& Mouse::Instance() {
    static Mouse instance;
    return instance;
}

Vector2D Mouse::GetPosition() const {
    return position;
}

Vector2D Mouse::GetWorldPosition() const {
    // TODO: Add camera offset when you implement camera system
    return position;
}

Vector2D Mouse::GetRelativeMotion() const {
    return relativeMotion;
}

bool Mouse::IsButtonPressed(int button) const {
    return (currentButtonState & SDL_BUTTON(button)) != 0;
}

bool Mouse::IsButtonJustPressed(int button) const {
    return ((currentButtonState & SDL_BUTTON(button)) != 0) &&
           ((previousButtonState & SDL_BUTTON(button)) == 0);
}

bool Mouse::IsButtonJustReleased(int button) const {
    return ((currentButtonState & SDL_BUTTON(button)) == 0) &&
           ((previousButtonState & SDL_BUTTON(button)) != 0);
}

void Mouse::Update() {
    previousButtonState = currentButtonState;

    int x, y;
    currentButtonState = SDL_GetMouseState(&x, &y);
    position = Vector2D(static_cast<float>(x), static_cast<float>(y));

    // Get relative motion
    int relX, relY;
    SDL_GetRelativeMouseState(&relX, &relY);
    relativeMotion = Vector2D(static_cast<float>(relX), static_cast<float>(relY));
}