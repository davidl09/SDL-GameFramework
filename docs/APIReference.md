# API Reference

## Core Classes

### Game
The main engine class that manages the game loop and core systems.

```cpp
class Game {
public:
    static Game& Instance();
    
    bool Initialize(const std::string& title = "SDL Game",
                   int width = 800,
                   int height = 600);
    void Run();
    void Quit();
    
    SDL_Renderer* GetRenderer() const;
    int GetWindowWidth() const;
    int GetWindowHeight() const;
    float GetDeltaTime() const;
    bool IsRunning() const;
    
    void ChangeScene(std::shared_ptr<Scene> newScene);
};
```

### Scene
Base class for game scenes that manage collections of game objects.

```cpp
class Scene {
public:
    virtual void OnEnter();
    virtual void OnExit();
    virtual void Update(float deltaTime);
    virtual void Render();
    
    void AddGameObject(const std::shared_ptr<GameObject>& gameObject);
    void RemoveGameObject(const std::shared_ptr<GameObject>& gameObject);
    std::vector<std::shared_ptr<GameObject>> GetGameObjectsByTag(const std::string& tag);
    std::shared_ptr<GameObject> GetGameObjectByTag(const std::string& tag);
    
    void SetDebugDrawEnabled(bool enabled);
    bool IsDebugDrawEnabled() const;
    
protected:
    virtual void OnCollision(GameObject* first, GameObject* second);
};
```

### GameObject
Base class for all game entities.

```cpp
class GameObject {
public:
    explicit GameObject(std::string tag = "");
    virtual ~GameObject();
    
    virtual void Update(float deltaTime);
    virtual void Render();
    
    template<typename T>
    std::shared_ptr<T> AddComponent();
    
    template<typename T>
    std::shared_ptr<T> GetComponent();
    
    void SetTag(const std::string& tag);
    const std::string& GetTag() const;
    bool IsActive() const;
    void SetActive(bool active);
    
    virtual void OnCollisionEnter(GameObject* other);
    virtual void OnCollisionExit(GameObject* other);
};
```

## Components

### Transform
Handles position, rotation, and scale.

```cpp
class Transform {
public:
    void SetPosition(float x, float y);
    void SetPosition(const Vector2D& position);
    void Translate(const Vector2D& translation);
    void SetRotation(float degrees);
    void Rotate(float degrees);
    void SetScale(float x, float y);
    void SetScale(const Vector2D& scale);
    
    const Vector2D& GetPosition() const;
    float GetRotation() const;
    const Vector2D& GetScale() const;
    
    Vector2D Forward() const;
    Vector2D Right() const;
};
```

### Sprite
Handles rendering and animations.

```cpp
class Sprite {
public:
    void SetTexture(const std::string& texturePath);
    void SetSourceRect(int x, int y, int width, int height);
    void SetColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
    void SetBlendMode(SDL_BlendMode blendMode);
    void SetFlip(SDL_RendererFlip flip);
    
    void AddAnimation(const std::string& name, 
                     const std::vector<SDL_Rect>& frames,
                     float frameTime);
    void PlayAnimation(const std::string& name, bool loop = false);
    void StopAnimation();
    bool IsAnimationPlaying() const;
};
```

### Collider
Provides collision detection.

```cpp
class Collider {
public:
    void SetBounds(float x, float y, float width, float height);
    void SetBounds(const SDL_Rect& bounds);
    void SetOffset(const Vector2D& offset);
    void SetTrigger(bool isTrigger);
    
    const SDL_Rect& GetBounds() const;
    const Vector2D& GetOffset() const;
    bool IsTrigger() const;
    
    bool CheckCollision(const Collider& other) const;
};
```

## Managers

### AssetManager
Handles loading and managing game assets.

```cpp
class AssetManager {
public:
    static AssetManager& Instance();
    
    bool LoadTexture(const std::string& name, const std::string& filepath);
    SDL_Texture* GetTexture(const std::string& name);
    void UnloadTexture(const std::string& name);
    void UnloadAllTextures();
};
```

### AudioManager
Handles sound effects and music.

```cpp
class AudioManager {
public:
    static AudioManager& Instance();
    
    bool Initialize();
    void Shutdown();
    
    bool LoadSound(const std::string& name, const std::string& filepath);
    bool LoadMusic(const std::string& name, const std::string& filepath);
    
    void PlaySound(const std::string& name, int volume = 128);
    void PlayMusic(const std::string& name, bool loop = false);
    void StopMusic();
    void PauseMusic();
    void ResumeMusic();
    
    void SetMasterVolume(int volume);
    void SetMusicVolume(int volume);
    void SetSoundVolume(int volume);
};
```

## Input

### Keyboard
Static class for keyboard input.

```cpp
class Keyboard {
public:
    static bool IsKeyPressed(SDL_Scancode key);
    static bool IsKeyDown(SDL_Scancode key);
    static bool IsKeyUp(SDL_Scancode key);
};
```

### Mouse
Static class for mouse input.

```cpp
class Mouse {
public:
    static bool IsButtonPressed(Uint8 button);
    static bool IsButtonDown(Uint8 button);
    static bool IsButtonUp(Uint8 button);
    static Vector2D GetPosition();
    static Vector2D GetWorldPosition();
    static Vector2D GetMovement();
};
```

## Utility Classes

### Vector2D
2D vector math utility.

```cpp
class Vector2D {
public:
    float x, y;
    
    Vector2D(float x = 0.0f, float y = 0.0f);
    
    Vector2D operator+(const Vector2D& other) const;
    Vector2D operator-(const Vector2D& other) const;
    Vector2D operator*(float scalar) const;
    Vector2D operator/(float scalar) const;
    
    Vector2D& operator+=(const Vector2D& other);
    Vector2D& operator-=(const Vector2D& other);
    Vector2D& operator*=(float scalar);
    Vector2D& operator/=(float scalar);
    
    float Length() const;
    float LengthSquared() const;
    void Normalize();
    Vector2D Normalized() const;
    
    float Dot(const Vector2D& other) const;
    float Cross(const Vector2D& other) const;
    float Angle(const Vector2D& other) const;
    
    static Vector2D Zero();
    static Vector2D One();
    static Vector2D Up();
    static Vector2D Down();
    static Vector2D Left();
    static Vector2D Right();
};
```

## Constants and Enums

### Layer Constants
```cpp
namespace Layers {
    const std::string DEFAULT = "default";
    const std::string PLAYER = "player";
    const std::string ENEMY = "enemy";
    const std::string PROJECTILE = "projectile";
    const std::string TRIGGER = "trigger";
}
```

### Input Constants
```cpp
namespace Input {
    const SDL_Scancode MOVE_UP = SDL_SCANCODE_W;
    const SDL_Scancode MOVE_DOWN = SDL_SCANCODE_S;
    const SDL_Scancode MOVE_LEFT = SDL_SCANCODE_A;
    const SDL_Scancode MOVE_RIGHT = SDL_SCANCODE_D;
    const SDL_Scancode JUMP = SDL_SCANCODE_SPACE;
    const SDL_Scancode INTERACT = SDL_SCANCODE_E;
}
```

### Render Layers
```cpp
enum class RenderLayer {
    BACKGROUND = 0,
    WORLD = 100,
    ENTITIES = 200,
    EFFECTS = 300,
    UI = 400
};
```
