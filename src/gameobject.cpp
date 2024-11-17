#include "gameobject.h"

#include <game.h>
#include <utility>

GameObject::GameObject(std::string  tag)
    : tag(std::move(tag))
    , isActive(true)
{
}

void GameObject::Update(float deltaTime) {
    // Base class doesn't implement any behavior
}

void GameObject::Render() {
    if (!isActive || !sprite) return;

    sprite->Render(transform);

    // Debug render for collider if it exists
#ifdef _DEBUG
    if (collider) {
        collider->RenderDebug(renderer, transform);
    }
#endif
}

void GameObject::SetSprite(std::shared_ptr<Sprite> sprite) {
    this->sprite = std::move(sprite);
}

void GameObject::SetCollider(std::shared_ptr<Collider> collider) {
    this->collider = std::move(collider);
}

bool GameObject::CheckCollision(const GameObject& other) const {
    if (!isActive || !other.IsActive()) return false;
    if (!collider || !other.collider) return false;

    return collider->CheckCollision(*other.collider, transform, other.GetTransform());
}