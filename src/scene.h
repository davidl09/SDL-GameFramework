/**
 * @file scene.h
 * @brief Scene management class that handles collections of game objects and their interactions
 * 
 * The Scene class serves as a container and manager for GameObjects. It handles:
 * - GameObject lifecycle management
 * - Collision detection and resolution
 * - Update and render loops
 * - Tag-based object management
 */
#pragma once
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <memory>
#include <SDL2/SDL.h>

#include "gameobject.h"

class Scene {
public:
    Scene() = default;
    virtual ~Scene();

    // Prevent copying and assignment
    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;

    /**
     * @brief Called when the scene becomes active
     * Override this to initialize scene-specific resources
     */
    virtual void OnEnter() {}

    /**
     * @brief Called when the scene is being exited
     * Override this to cleanup scene-specific resources
     */
    virtual void OnExit() {}

    /**
     * @brief Update all game objects in the scene
     * @param deltaTime Time elapsed since last frame in seconds
     */
    virtual void Update(float deltaTime);

    /**
     * @brief Render all game objects in the scene
     */
    virtual void Render();

    /**
     * @brief Add a game object to the scene
     * @param gameObject Shared pointer to the game object to add
     */
    void AddGameObject(const std::shared_ptr<GameObject>& gameObject);

    /**
     * @brief Remove a game object from the scene
     * @param gameObject Shared pointer to the game object to remove
     */
    void RemoveGameObject(const std::shared_ptr<GameObject>& gameObject);

    /**
     * @brief Get all game objects with a specific tag
     * @param tag The tag to search for
     * @return Vector of game objects with the specified tag
     */
    std::vector<std::shared_ptr<GameObject>> GetGameObjectsByTag(const std::string& tag);

    /**
     * @brief Get the first game object with a specific tag
     * @param tag The tag to search for
     * @return Shared pointer to the first matching game object, or nullptr if none found
     */
    std::shared_ptr<GameObject> GetGameObjectByTag(const std::string& tag);

    /**
     * @brief Enable or disable debug drawing
     * @param enabled True to enable debug drawing
     */
    void SetDebugDrawEnabled(bool enabled) { debugDrawEnabled = enabled; }

    /**
     * @brief Check if debug drawing is enabled
     * @return True if debug drawing is enabled
     */
    bool IsDebugDrawEnabled() const { return debugDrawEnabled; }

protected:
    /**
     * @brief Called when two objects collide
     * Override this to handle collision events
     * @param first First colliding object
     * @param second Second colliding object
     */
    virtual void OnCollision(GameObject* first, GameObject* second) {}

private:
    // Collision pair tracking using weak pointers
    using CollisionPair = std::pair<std::weak_ptr<GameObject>, std::weak_ptr<GameObject>>;

    /**
     * @brief Hash function for collision pairs
     */
    struct WeakPtrPairHash {
        std::size_t operator()(const CollisionPair& p) const {
            auto p1 = p.first.lock();
            auto p2 = p.second.lock();
            if (!p1 || !p2) return 0;

            auto h1 = std::hash<GameObject*>{}(p1.get());
            auto h2 = std::hash<GameObject*>{}(p2.get());
            return h1 ^ (h2 << 1);
        }
    };

    /**
     * @brief Equality comparison for collision pairs
     */
    struct WeakPtrPairEqual {
        bool operator()(const CollisionPair& a, const CollisionPair& b) const {
            auto a1 = a.first.lock();
            auto a2 = a.second.lock();
            auto b1 = b.first.lock();
            auto b2 = b.second.lock();

            if (!a1 || !a2 || !b1 || !b2) return false;

            return (a1.get() == b1.get() && a2.get() == b2.get()) ||
                   (a1.get() == b2.get() && a2.get() == b1.get());
        }
    };

    /**
     * @brief Information about a tagged object
     */
    struct TaggedObjectInfo {
        std::weak_ptr<GameObject> object;  ///< Weak reference to the game object
        size_t index;                      ///< Index in gameObjects vector
    };

    // Main storage containers
    std::vector<std::shared_ptr<GameObject>> gameObjects;  ///< All game objects in the scene
    std::unordered_map<std::string, std::vector<TaggedObjectInfo>> taggedObjects;  ///< Objects organized by tag
    std::unordered_set<CollisionPair, WeakPtrPairHash, WeakPtrPairEqual> activeCollisions;  ///< Currently active collisions

    // State flags
    bool isProcessingCollisions = false;  ///< Flag to prevent recursive collision processing
    bool debugDrawEnabled = false;        ///< Flag for debug visualization

    /**
     * @brief Check for collisions between all objects
     */
    void CheckCollisions();

    /**
     * @brief Process collision between two objects
     */
    void ProcessCollisionPair(const std::shared_ptr<GameObject>& first,
                            const std::shared_ptr<GameObject>& second,
                            std::unordered_set<CollisionPair, WeakPtrPairHash, WeakPtrPairEqual>& currentCollisions);

    /**
     * @brief Handle collision exit events
     */
    void ProcessCollisionExits(const std::unordered_set<CollisionPair, WeakPtrPairHash, WeakPtrPairEqual>& currentCollisions);

    /**
     * @brief Safely call collision handlers
     */
    void SafeCallCollisionHandlers(const std::shared_ptr<GameObject>& first,
                                 const std::shared_ptr<GameObject>& second);

    /**
     * @brief Safely call collision exit handlers
     */
    void SafeCallCollisionExitHandlers(const std::shared_ptr<GameObject>& first,
                                     const std::shared_ptr<GameObject>& second);

    /**
     * @brief Create an ordered pair of game objects for collision tracking
     */
    CollisionPair CreateOrderedPair(const std::shared_ptr<GameObject>& first,
                                  const std::shared_ptr<GameObject>& second);

    /**
     * @brief Register a game object's tag
     */
    void RegisterGameObjectTag(const std::shared_ptr<GameObject>& gameObject);

    /**
     * @brief Unregister a game object's tag
     */
    void UnregisterGameObjectTag(const std::shared_ptr<GameObject>& gameObject);

    /**
     * @brief Clean up invalid tag references
     */
    void CleanupTags();

    /**
     * @brief Draw debug visualization for collisions
     */
    void DrawDebugCollisions(SDL_Renderer* renderer);
};