#include "game.h"
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <stdexcept>

Game::Game(const std::string& title, int width, int height)
    : title(title)
    , width(width)
    , height(height)
    , isRunning(false)
    , window(nullptr)
    , renderer(nullptr)
    , lastFrameTime(0)
    , deltaTime(0.0f)
    , targetFrameRate(60.0f)
    , frameDelay(1000.0f / targetFrameRate)
{
}

Game::~Game() {
    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }
    if (window) {
        SDL_DestroyWindow(window);
    }

    IMG_Quit();
    Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();
}

bool Game::Initialize() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        throw std::runtime_error(SDL_GetError());
    }

    window = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        throw std::runtime_error(SDL_GetError());
    }

    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (!renderer) {
        throw std::runtime_error(SDL_GetError());
    }

    // Initialize SDL_image
    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        throw std::runtime_error(IMG_GetError());
    }

    // Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        throw std::runtime_error(Mix_GetError());
    }

    isRunning = true;
    lastFrameTime = SDL_GetTicks();

    return true;
}

void Game::Run() {
    while (isRunning) {
        ProcessInput();
        Update();
        Render();
        CalculateDeltaTime();

        // Frame rate limiting
        int frameTime = SDL_GetTicks() - lastFrameTime;
        if (frameTime < frameDelay) {
            SDL_Delay(static_cast<Uint32>(frameDelay - frameTime));
        }
    }
}

void Game::Quit() {
    isRunning = false;
}

void Game::ChangeScene(std::shared_ptr<Scene> newScene) {
    currentScene = newScene;
}

void Game::ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                isRunning = false;
                break;
        }
    }
}

void Game::Update() {
    if (currentScene) {
        currentScene->Update(deltaTime);
    }
}

void Game::Render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    if (currentScene) {
        currentScene->Render(renderer);
    }

    SDL_RenderPresent(renderer);
}

void Game::CalculateDeltaTime() {
    Uint32 currentTime = SDL_GetTicks();
    deltaTime = (currentTime - lastFrameTime) / 1000.0f;
    lastFrameTime = currentTime;
}