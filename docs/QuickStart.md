# Quick Start Guide

## Installation

1. **Prerequisites**
   - CMake 3.10 or later
   - SDL2 development libraries
   - C++17 compatible compiler

2. **Building the Engine**
```bash
# Clone the repository
git clone https://github.com/yourusername/sdl2-engine.git

# Create build directory
cd sdl2-engine
mkdir build
cd build

# Generate build files and compile
cmake ..
make
```

## Creating Your First Game

1. **Create a New Scene**
```cpp
// MyGameScene.h
#include "scene.h"

class MyGameScene : public Scene {
public:
    void OnEnter() override {
        // Initialize your scene
        CreatePlayer();
        CreateEnemies();
    }
    
    void Update(float deltaTime) override {
        Scene::Update(deltaTime);  // Don't forget to call base class update
        // Add your game logic here
    }
    
private:
    void CreatePlayer() {
        auto player = std::make_shared<GameObject>("player");
        player->GetTransform().SetPosition(400, 300);
        
        auto sprite = player->AddComponent<Sprite>();
        sprite->SetTexture("player.png");
        
        auto collider = player->AddComponent<Collider>();
        collider->SetBounds(0, 0, 32, 32);
        
        AddGameObject(player);
    }
};
```

2. **Initialize the Game**
```cpp
// main.cpp
#include "game.h"
#include "MyGameScene.h"

int main(int argc, char* argv[]) {
    auto& game = Game::Instance();
    
    if (!game.Initialize("My Game", 800, 600)) {
        return -1;
    }
    
    auto scene = std::make_shared<MyGameScene>();
    game.ChangeScene(scene);
    
    game.Run();
    return 0;
}
```

## Common Tasks

### Loading Assets
```cpp
// Load textures
AssetManager::Instance().LoadTexture("player", "assets/player.png");
AssetManager::Instance().LoadTexture("enemy", "assets/enemy.png");

// Load audio
AudioManager::Instance().LoadSound("shoot", "assets/shoot.wav");
AudioManager::Instance().LoadMusic("background", "assets/music.mp3");
```

### Handling Input
```cpp
void Update(float deltaTime) override {
    // Keyboard input
    if (Keyboard::IsKeyPressed(SDL_SCANCODE_SPACE)) {
        // Space bar pressed
        Shoot();
    }
    
    // Mouse input
    if (Mouse::IsButtonPressed(SDL_BUTTON_LEFT)) {
        Vector2D mousePos = Mouse::GetPosition();
        // Left click at mousePos
    }
    
    // Movement
    Vector2D movement;
    if (Keyboard::IsKeyPressed(SDL_SCANCODE_W)) movement.y -= 1;
    if (Keyboard::IsKeyPressed(SDL_SCANCODE_S)) movement.y += 1;
    if (Keyboard::IsKeyPressed(SDL_SCANCODE_A)) movement.x -= 1;
    if (Keyboard::IsKeyPressed(SDL_SCANCODE_D)) movement.x += 1;
    
    if (movement.Length() > 0) {
        movement.Normalize();
        movement *= speed * deltaTime;
        GetTransform().Translate(movement);
    }
}
```

### Collision Detection
```cpp
class Player : public GameObject {
public:
    void OnCollisionEnter(GameObject* other) override {
        if (other->GetTag() == "enemy") {
            // Handle collision with enemy
            TakeDamage();
        }
        else if (other->GetTag() == "powerup") {
            // Handle powerup collection
            CollectPowerup(other);
        }
    }
};
```

### Playing Audio
```cpp
// Play sound effects
AudioManager::Instance().PlaySound("shoot");

// Play background music
AudioManager::Instance().PlayMusic("background", true);  // true for looping
```

### Creating Animations
```cpp
auto sprite = gameObject->GetComponent<Sprite>();
sprite->SetTexture("player_sheet.png");

// Add animation frames (x, y, width, height)
std::vector<SDL_Rect> walkFrames;
walkFrames.push_back({0, 0, 32, 32});
walkFrames.push_back({32, 0, 32, 32});
walkFrames.push_back({64, 0, 32, 32});

sprite->AddAnimation("walk", walkFrames, 0.1f);  // 0.1f seconds per frame
sprite->PlayAnimation("walk", true);  // true for looping
```

## Best Practices

1. **Scene Organization**
   - Keep scene initialization in OnEnter()
   - Clean up resources in OnExit()
   - Use tags to organize game objects

2. **Performance**
   - Pool frequently created/destroyed objects
   - Use appropriate data structures
   - Minimize allocations during gameplay

3. **Code Structure**
   - Separate game logic into components
   - Use inheritance wisely
   - Keep the Update() methods clean and focused

4. **Debug Tools**
   - Use SetDebugDrawEnabled(true) to visualize colliders
   - Add debug logging where needed
   - Create debug controls for testing

## Common Issues

1. **Memory Leaks**
   - Always use smart pointers for game objects
   - Clean up SDL resources in destructors
   - Remove listeners and observers properly

2. **Performance**
   - Too many objects in the scene
   - Inefficient collision checking
   - Resource loading during gameplay

3. **Input Handling**
   - Input not working in window focus
   - Key repeat issues
   - Mouse position conversion

## Next Steps

1. Explore the [Components Documentation](Components.md)
2. Check out the [API Reference](APIReference.md)
3. Look at the [Example Projects](Examples.md)
4. Join the community and contribute!
