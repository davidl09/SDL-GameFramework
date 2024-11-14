#include "GameObject.h"

GameObject::GameObject(const std::string& tag)
    : tag(tag)
    , isActive(true)
{
}

void GameObject::Update(float deltaTime) {
    // Base class doesn't implement any behavior
}

void GameObject::Render(SDL_Renderer* renderer) {
    if (!isActive || !sprite) return;

    sprite->Render(renderer, transform);

    // Debug render for collider if it exists
#ifdef _DEBUG
    if (collider) {
        collider->RenderDebug(renderer, transform);
    }
#endif
}

void GameObject::SetSprite(std::shared_ptr<Sprite> sprite) {
    this->sprite = sprite;
}

void GameObject::SetCollider(std::shared_ptr<Collider> collider) {
    this->collider = collider;
}

bool GameObject::CheckCollision(const GameObject& other) const {
    if (!isActive || !other.IsActive()) return false;
    if (!collider || !other.collider) return false;

    return collider->CheckCollision(*other.collider, transform, other.GetTransform());
}