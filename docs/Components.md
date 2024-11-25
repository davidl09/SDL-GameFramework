# Components Documentation

## Overview
The engine uses a component-based architecture where each GameObject can have multiple components that define its behavior and capabilities.

## Core Components

### Transform Component
The Transform component handles the position, rotation, and scale of game objects.

```cpp
class Transform {
    Vector2D position;    // Position in 2D space
    float rotation;       // Rotation in degrees
    Vector2D scale;       // Scale in x and y directions
};
```

#### Usage
```cpp
auto gameObject = std::make_shared<GameObject>();
auto& transform = gameObject->GetTransform();
transform.SetPosition(100, 200);
transform.SetRotation(45.0f);
transform.SetScale(2.0f, 2.0f);
```

### Sprite Component
The Sprite component handles rendering textures and animations.

```cpp
class Sprite {
    SDL_Texture* texture;    // The texture to render
    SDL_Rect sourceRect;     // Source rectangle for sprite sheets
    SDL_Rect destRect;       // Destination rectangle for rendering
};
```

#### Usage
```cpp
auto gameObject = std::make_shared<GameObject>();
auto sprite = gameObject->AddComponent<Sprite>();
sprite->SetTexture("player.png");
sprite->SetSourceRect(0, 0, 32, 32);  // For sprite sheets
```

### Collider Component
The Collider component provides collision detection capabilities.

```cpp
class Collider {
    SDL_Rect bounds;         // Collision bounds
    bool isTrigger;          // If true, doesn't block movement
    std::string layer;       // Collision layer
};
```

#### Usage
```cpp
auto gameObject = std::make_shared<GameObject>();
auto collider = gameObject->AddComponent<Collider>();
collider->SetBounds(0, 0, 32, 32);
collider->SetTrigger(true);
collider->SetLayer("enemies");
```

## Audio Component
The Audio component manages sound effects and music playback.

```cpp
class AudioManager {
    void PlaySound(const std::string& soundFile);
    void PlayMusic(const std::string& musicFile);
    void StopMusic();
    void SetVolume(int volume);  // 0-128
};
```

#### Usage
```cpp
AudioManager::Instance().PlaySound("explosion.wav");
AudioManager::Instance().PlayMusic("background.mp3");
```

## Creating Custom Components

You can create custom components by inheriting from the Component base class:

```cpp
class CustomComponent : public Component {
public:
    void Update(float deltaTime) override {
        // Custom update logic
    }
    
    void Render() override {
        // Custom rendering logic
    }
};
```

### Usage
```cpp
auto gameObject = std::make_shared<GameObject>();
auto customComponent = gameObject->AddComponent<CustomComponent>();
```

## Component Lifecycle

Components follow this lifecycle:

1. **Creation**: When added to a GameObject using AddComponent<T>()
2. **Initialization**: When the GameObject is added to a Scene
3. **Update**: Called every frame with the elapsed time
4. **Render**: Called every frame after Update
5. **Destruction**: When the GameObject is destroyed or removed from the Scene

## Best Practices

1. **Component Dependencies**
   - Check for required components in initialization
   - Use GetComponent<T>() to access other components
   - Handle missing dependencies gracefully

2. **Performance**
   - Keep Update() methods lightweight
   - Cache component references rather than getting them every frame
   - Use appropriate data structures for component storage

3. **Memory Management**
   - Use smart pointers for component ownership
   - Clean up resources in component destructors
   - Remove unused components to free memory

4. **Communication**
   - Use the Scene's messaging system for inter-object communication
   - Implement appropriate collision callbacks
   - Use events for loose coupling between components
