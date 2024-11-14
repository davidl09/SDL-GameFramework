#pragma once
#include <SDL2/SDL.h>
#include <array>

class Keyboard {
public:
    static Keyboard& Instance();

    // Delete copy constructor and assignment operator
    Keyboard(const Keyboard&) = delete;
    Keyboard& operator=(const Keyboard&) = delete;

    // Keyboard state
    bool IsKeyPressed(SDL_Scancode key) const;
    bool IsKeyJustPressed(SDL_Scancode key) const;
    bool IsKeyJustReleased(SDL_Scancode key) const;

    // Should be called each frame to update key states
    void Update();

private:
    Keyboard() = default;

    static constexpr int NUM_KEYS = SDL_NUM_SCANCODES;
    std::array<Uint8, NUM_KEYS> currentKeyState{};
    std::array<Uint8, NUM_KEYS> previousKeyState{};
};
