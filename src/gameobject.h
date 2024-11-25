/**
 * @file gameobject.h
 * @brief Base class for all game entities using a component-based architecture
 * 
 * The GameObject class serves as a container for components that define its behavior
 * and properties. It follows the Component Pattern where each behavior or capability
 * is encapsulated in a separate component that can be added or removed at runtime.
 */
#pragma once
#include <memory>
#include <string>
#include "transform.h"
#include "sprite.h"
#include "collider.h"

/**
 * @class GameObject
 * @brief Base class for all game entities
 * 
 * The GameObject class provides a basic structure for game entities, including
 * component management, update and render methods, and collision detection.
 */
class GameObject {
public:
    /**
     * @brief Constructor for GameObject
     * @param tag Optional tag for the object
     */
    explicit GameObject(std::string tag = "");

    /**
     * @brief Destructor for GameObject
     */
    virtual ~GameObject() = default;

    /**
     * @brief Update the game object and its components
     * @param deltaTime Time elapsed since last frame
     */
    virtual void Update(float deltaTime);

    /**
     * @brief Render the game object and its components
     */
    virtual void Render();

    /**
     * @brief Set the sprite component for this game object
     * @param sprite Shared pointer to the sprite component
     */
    void SetSprite(std::shared_ptr<Sprite> sprite);

    /**
     * @brief Set the collider component for this game object
     * @param collider Shared pointer to the collider component
     */
    void SetCollider(std::shared_ptr<Collider> collider);

    /**
     * @brief Get the transform component for this game object
     * @return Constant reference to the transform component
     */
    [[nodiscard]] const Transform& GetTransform() const { return transform; }

    /**
     * @brief Get the transform component for this game object (non-const)
     * @return Reference to the transform component
     */
    Transform& GetTransform() { return transform; }

    /**
     * @brief Get the tag for this game object
     * @return Constant reference to the tag
     */
    [[nodiscard]] const std::string& GetTag() const { return tag; }

    /**
     * @brief Get the sprite component for this game object
     * @return Shared pointer to the sprite component
     */
    [[nodiscard]] std::shared_ptr<Sprite> GetSprite() const { return sprite; }

    /**
     * @brief Get the collider component for this game object
     * @return Shared pointer to the collider component
     */
    [[nodiscard]] std::shared_ptr<Collider> GetCollider() const { return collider; }

    /**
     * @brief Check if the game object is active
     * @return True if the object is active
     */
    [[nodiscard]] bool IsActive() const { return isActive; }

    /**
     * @brief Set whether the game object is active
     * @param active True to activate, false to deactivate
     */
    void SetActive(const bool active) { isActive = active; }

    /**
     * @brief Check for collision with another game object
     * @param other Game object to check for collision
     * @return True if collision is detected
     */
    [[nodiscard]] bool CheckCollision(const GameObject& other) const;

    /**
     * @brief Virtual method called when collision is detected
     * @param other Game object that collided with this object
     */
    virtual void OnCollisionEnter(GameObject* other) {}

    /**
     * @brief Virtual method called when collision is no longer detected
     * @param other Game object that stopped colliding with this object
     */
    virtual void OnCollisionExit(GameObject* other) {}

protected:
    Transform transform; ///< Transform component for this game object
    std::shared_ptr<Sprite> sprite; ///< Sprite component for this game object
    std::shared_ptr<Collider> collider; ///< Collider component for this game object
    std::string tag; ///< Tag for this game object
    bool isActive; ///< Active state flag
};

/**
 * @brief Helper template for safe casting between GameObject types
 * @tparam T Type to cast to
 * @param obj GameObject to cast
 * @return Pointer to the casted object, or nullptr if cast fails
 */
template<typename T>
T* GameObjectCast(GameObject* obj) {
    return dynamic_cast<T*>(obj);
}