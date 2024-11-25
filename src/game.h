/**
 * @file game.h
 * @brief Core game engine class that manages the game loop, window, and renderer.
 * 
 * The Game class is implemented as a singleton and serves as the main entry point
 * for the engine. It handles initialization of SDL subsystems, manages the main
 * game loop, and provides access to core engine components.
 */
#pragma once
#include <SDL2/SDL.h>
#include <memory>
#include <random>
#include <string>
#include "scene.h"

class Game {
public:
    // Delete copy constructor and assignment operator to ensure singleton pattern
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    /**
     * @brief Get the singleton instance of the Game class
     * @return Reference to the Game instance
     */
    static Game& Instance() {
        static Game instance;
        return instance;
    }

    /**
     * @brief Initialize the game engine
     * @param title Window title
     * @param width Window width in pixels
     * @param height Window height in pixels
     * @return true if initialization succeeded, false otherwise
     */
    bool Initialize(const std::string& title = "SDL Game",
                   int width = 800,
                   int height = 600);

    /**
     * @brief Start the main game loop
     * This method runs the game loop until Quit() is called
     */
    void Run();

    /**
     * @brief Quit the game and cleanup resources
     */
    void Quit();

    // Global accessors
    /**
     * @brief Get the SDL renderer
     * @return Pointer to the SDL renderer
     */
    [[nodiscard]] SDL_Renderer* GetRenderer() const { return renderer; }

    /**
     * @brief Get the window width
     * @return Window width in pixels
     */
    [[nodiscard]] int GetWindowWidth() const { return width; }

    /**
     * @brief Get the window height
     * @return Window height in pixels
     */
    [[nodiscard]] int GetWindowHeight() const { return height; }

    /**
     * @brief Get the time elapsed since last frame
     * @return Delta time in seconds
     */
    [[nodiscard]] float GetDeltaTime() const { return deltaTime; }

    /**
     * @brief Check if the game is currently running
     * @return true if the game loop is active
     */
    [[nodiscard]] bool IsRunning() const { return isRunning; }

    /**
     * @brief Change the active scene
     * @param newScene Shared pointer to the new scene
     */
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

    /**
     * @brief Process input events
     */
    void ProcessInput();

    /**
     * @brief Update game logic
     */
    void Update();

    /**
     * @brief Render the current frame
     */
    void Render();

    /**
     * @brief Calculate time elapsed since last frame
     */
    void CalculateDeltaTime();

    std::string title;    ///< Window title
    int width;           ///< Window width
    int height;          ///< Window height
    bool isRunning;      ///< Game loop state

    SDL_Window* window;      ///< SDL window handle
    SDL_Renderer* renderer;  ///< SDL renderer handle
    std::shared_ptr<Scene> currentScene;  ///< Currently active scene

    Uint32 lastFrameTime;   ///< Timestamp of last frame
    float deltaTime;        ///< Time elapsed since last frame
    float targetFrameRate;  ///< Target frame rate (default: 60 FPS)
    float frameDelay;       ///< Delay between frames
};