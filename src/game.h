// game.h
#pragma once
#include <SDL2/SDL.h>
#include <memory>
#include <random>
#include <string>
#include "scene.h"

class Game {
public:
    // Delete copy constructor and assignment operator
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    // Singleton accessor
    static Game& Instance() {
        static Game instance;
        return instance;
    }

    // Initialize with window settings
    bool Initialize(const std::string& title = "SDL Game",
                   int width = 800,
                   int height = 600);
    void Run();
    void Quit();

    // Global accessors
    [[nodiscard]] SDL_Renderer* GetRenderer() const { return renderer; }
    [[nodiscard]] int GetWindowWidth() const { return width; }
    [[nodiscard]] int GetWindowHeight() const { return height; }
    [[nodiscard]] float GetDeltaTime() const { return deltaTime; }
    [[nodiscard]] bool IsRunning() const { return isRunning; }

    void ChangeScene(std::shared_ptr<Scene> newScene);

private:
    // Private constructor for singleton
    Game()
        : title("SDL Game")
        , width(800)
        , height(600)
        , isRunning(false)
        , window(nullptr)
        , renderer(nullptr)
        , lastFrameTime(0)
        , deltaTime(0.0f)
        , targetFrameRate(60.0f)
        , frameDelay(1000.0f / 60.0f)
    {}

    ~Game();

    void ProcessInput();
    void Update();
    void Render();
    void CalculateDeltaTime();

    std::string title;
    int width;
    int height;
    bool isRunning;

    SDL_Window* window;
    SDL_Renderer* renderer;
    std::shared_ptr<Scene> currentScene;

    Uint32 lastFrameTime;
    float deltaTime;
    float targetFrameRate;
    float frameDelay;
};