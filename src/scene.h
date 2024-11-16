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

    // Scene lifecycle methods
    virtual void OnEnter() {}
    virtual void OnExit() {}
    virtual void Update(float deltaTime);
    virtual void Render(SDL_Renderer* renderer);

    // GameObject management
    void AddGameObject(const std::shared_ptr<GameObject>& gameObject);
    void RemoveGameObject(const std::shared_ptr<GameObject>& gameObject);
    std::vector<std::shared_ptr<GameObject>> GetGameObjectsByTag(const std::string& tag);
    std::shared_ptr<GameObject> GetGameObjectByTag(const std::string& tag);

    // Debug options
    void SetDebugDrawEnabled(bool enabled) { debugDrawEnabled = enabled; }
    bool IsDebugDrawEnabled() const { return debugDrawEnabled; }

protected:
    virtual void OnCollision(GameObject* first, GameObject* second) {}

private:
    // Collision pair tracking using weak pointers
    using CollisionPair = std::pair<std::weak_ptr<GameObject>, std::weak_ptr<GameObject>>;

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

    // Tagged object tracking
    struct TaggedObjectInfo {
        std::weak_ptr<GameObject> object;
        size_t index;  // Index in gameObjects vector for quick access
    };

    // Main storage
    std::vector<std::shared_ptr<GameObject>> gameObjects;
    std::unordered_map<std::string, std::vector<TaggedObjectInfo>> taggedObjects;
    std::unordered_set<CollisionPair, WeakPtrPairHash, WeakPtrPairEqual> activeCollisions;

    // State flags
    bool isProcessingCollisions = false;
    bool debugDrawEnabled = false;

    // Internal methods
    void CheckCollisions();
    void ProcessCollisionPair(const std::shared_ptr<GameObject>& first,
                            const std::shared_ptr<GameObject>& second,
                            std::unordered_set<CollisionPair, WeakPtrPairHash, WeakPtrPairEqual>& currentCollisions);
    void ProcessCollisionExits(const std::unordered_set<CollisionPair, WeakPtrPairHash, WeakPtrPairEqual>& currentCollisions);
    void SafeCallCollisionHandlers(const std::shared_ptr<GameObject>& first,
                                 const std::shared_ptr<GameObject>& second);
    void SafeCallCollisionExitHandlers(const std::shared_ptr<GameObject>& first,
                                     const std::shared_ptr<GameObject>& second);
    CollisionPair CreateOrderedPair(const std::shared_ptr<GameObject>& first,
                                  const std::shared_ptr<GameObject>& second);

    void RegisterGameObjectTag(const std::shared_ptr<GameObject>& gameObject);
    void UnregisterGameObjectTag(const std::shared_ptr<GameObject>& gameObject);
    void CleanupTags();
    void DrawDebugCollisions(SDL_Renderer* renderer);
};