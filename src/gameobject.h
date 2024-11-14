#pragma once
#include <memory>
#include <string>
#include "transform.h"
#include "sprite.h"
#include "collider.h"

class GameObject {
public:
    GameObject(const std::string& tag = "");
    virtual ~GameObject() = default;

    virtual void Update(float deltaTime);
    virtual void Render(SDL_Renderer* renderer);

    // Component management
    void SetSprite(std::shared_ptr<Sprite> sprite);
    void SetCollider(std::shared_ptr<Collider> collider);

    // Getters
    const Transform& GetTransform() const { return transform; }
    Transform& GetTransform() { return transform; }
    const std::string& GetTag() const { return tag; }
    std::shared_ptr<Sprite> GetSprite() const { return sprite; }
    std::shared_ptr<Collider> GetCollider() const { return collider; }

    bool IsActive() const { return isActive; }
    void SetActive(bool active) { isActive = active; }

    // Collision checking
    bool CheckCollision(const GameObject& other) const;

    // Virtual collision handlers
    virtual void OnCollisionEnter(GameObject* other) {}
    virtual void OnCollisionExit(GameObject* other) {}

protected:
    Transform transform;
    std::shared_ptr<Sprite> sprite;
    std::shared_ptr<Collider> collider;
    std::string tag;
    bool isActive;
};

// Helper template for safe casting between GameObject types
template<typename T>
T* GameObjectCast(GameObject* obj) {
    return dynamic_cast<T*>(obj);
}