#pragma once
#include <memory>
#include <string>
#include "transform.h"
#include "sprite.h"
#include "collider.h"

class GameObject {
public:
    explicit GameObject(std::string  tag = "");
    virtual ~GameObject() = default;

    virtual void Update(float deltaTime);
    virtual void Render();

    // Component management
    void SetSprite(std::shared_ptr<Sprite> sprite);
    void SetCollider(std::shared_ptr<Collider> collider);

    // Getters
    [[nodiscard]] const Transform& GetTransform() const { return transform; }
    Transform& GetTransform() { return transform; }
    [[nodiscard]] const std::string& GetTag() const { return tag; }
    [[nodiscard]] std::shared_ptr<Sprite> GetSprite() const { return sprite; }
    [[nodiscard]] std::shared_ptr<Collider> GetCollider() const { return collider; }

    [[nodiscard]] bool IsActive() const { return isActive; }
    void SetActive(const bool active) { isActive = active; }

    // Collision checking
    [[nodiscard]] bool CheckCollision(const GameObject& other) const;

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