#pragma once
#include <SDL2/SDL.h>
#include <memory>
#include <string>
#include "scene.h"

class Game {
public:
    Game(const std::string& title, int width, int height);
    ~Game();

    bool Initialize();
    void Run();
    void Quit();

    SDL_Renderer* GetRenderer() const { return renderer; }
    void ChangeScene(std::shared_ptr<Scene> newScene);

    // Window dimensions accessors
    int GetWidth() const { return width; }
    int GetHeight() const { return height; }

    // Delta time accessor
    float GetDeltaTime() const { return deltaTime; }

private:
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