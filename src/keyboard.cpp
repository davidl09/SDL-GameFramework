#include "keyboard.h"
#include <cstring>

Keyboard& Keyboard::Instance() {
    static Keyboard instance;
    return instance;
}

bool Keyboard::IsKeyPressed(SDL_Scancode key) const {
    return currentKeyState[key] != 0;
}

bool Keyboard::IsKeyJustPressed(SDL_Scancode key) const {
    return currentKeyState[key] && !previousKeyState[key];
}

bool Keyboard::IsKeyJustReleased(SDL_Scancode key) const {
    return !currentKeyState[key] && previousKeyState[key];
}

void Keyboard::Update() {
    // Store previous state
    std::memcpy(previousKeyState.data(), currentKeyState.data(), NUM_KEYS);

    // Get current state
    const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);
    std::memcpy(currentKeyState.data(), keyboardState, NUM_KEYS);
}