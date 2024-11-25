# SDL2 Game Engine Documentation

## Overview
This is a lightweight, component-based 2D game engine built with SDL2. The engine provides essential features for creating 2D games including sprite rendering, collision detection, audio management, and input handling.

## Table of Contents
1. [Getting Started](#getting-started)
2. [Core Components](#core-components)
3. [Architecture](#architecture)
4. [Examples](#examples)
5. [API Reference](#api-reference)

## Getting Started

### Prerequisites
- SDL2
- C++17 or later
- CMake 3.10 or later

### Building the Engine
```bash
mkdir build
cd build
cmake ..
make
```

### Basic Usage
```cpp
#include "game.h"
#include "scene.h"

class MyGameScene : public Scene {
public:
    void OnEnter() override {
        // Initialize your scene
        auto player = std::make_shared<GameObject>();
        player->AddComponent<Transform>();
        player->AddComponent<Sprite>();
        player->AddComponent<Collider>();
        AddGameObject(player);
    }
    
    void Update(float deltaTime) override {
        // Update game logic
    }
};

int main() {
    // Initialize the engine
    if (!Game::Instance().Initialize("My Game", 800, 600)) {
        return -1;
    }
    
    // Create and set initial scene
    auto scene = std::make_shared<MyGameScene>();
    Game::Instance().ChangeScene(scene);
    
    // Start the game loop
    Game::Instance().Run();
    
    return 0;
}
```

## Core Components

### Game Class
The central manager class that handles the game loop, window management, and scene transitions.

### Scene
Manages collections of game objects and their interactions. Handles updates, rendering, and collision detection.

### GameObject
Base class for all game entities. Uses a component-based architecture for flexibility.

### Components
- **Transform**: Handles position, rotation, and scale
- **Sprite**: Manages rendering of textures
- **Collider**: Provides collision detection
- **AudioManager**: Handles sound effects and music
- **AssetManager**: Manages loading and unloading of resources

### Input System
- **Keyboard**: Handles keyboard input
- **Mouse**: Handles mouse input and position

## Architecture

### Component-Based Design
The engine uses a component-based architecture where GameObjects are containers for various components that define their behavior and properties.

```
GameObject
├── Transform
├── Sprite
├── Collider
└── Custom Components
```

### Scene Management
Scenes contain collections of GameObjects and manage their lifecycle:
- OnEnter: Called when scene becomes active
- Update: Called every frame
- Render: Handles drawing
- OnExit: Called when leaving the scene

### Collision System
Uses AABB (Axis-Aligned Bounding Box) collision detection with the following features:
- Collision enter/exit events
- Debug visualization
- Tag-based filtering

## Examples

### Creating a Simple Game Object
```cpp
auto gameObject = std::make_shared<GameObject>();
gameObject->AddComponent<Transform>()->SetPosition(100, 100);
auto sprite = gameObject->AddComponent<Sprite>();
sprite->SetTexture("player.png");
```

### Handling Input
```cpp
void Update(float deltaTime) override {
    if (Keyboard::IsKeyPressed(SDL_SCANCODE_SPACE)) {
        // Handle space key
    }
    
    Vector2D mousePos = Mouse::GetPosition();
    if (Mouse::IsButtonPressed(SDL_BUTTON_LEFT)) {
        // Handle left click
    }
}
```

### Playing Audio
```cpp
AudioManager::Instance().PlaySound("explosion.wav");
AudioManager::Instance().PlayMusic("background.mp3");
```

## API Reference

### Game
```cpp
class Game {
    static Game& Instance();
    bool Initialize(const std::string& title, int width, int height);
    void Run();
    void Quit();
    void ChangeScene(std::shared_ptr<Scene> newScene);
};
```

### Scene
```cpp
class Scene {
    virtual void OnEnter();
    virtual void OnExit();
    virtual void Update(float deltaTime);
    virtual void Render();
    void AddGameObject(const std::shared_ptr<GameObject>& gameObject);
    void RemoveGameObject(const std::shared_ptr<GameObject>& gameObject);
};
```

### GameObject
```cpp
class GameObject {
    template<typename T>
    std::shared_ptr<T> AddComponent();
    
    template<typename T>
    std::shared_ptr<T> GetComponent();
    
    void SetTag(const std::string& tag);
    const std::string& GetTag() const;
};
```

For more detailed information about specific components and features, please refer to the individual documentation files in the `/docs` directory.
