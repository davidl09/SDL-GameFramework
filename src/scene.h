#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include <unordered_set>

#include "gameobject.h"

class Scene {
public:
    Scene() = default;
    virtual ~Scene() = default;

    virtual void OnEnter() {}
    virtual void OnExit() {}
    virtual void Update(float deltaTime);
    virtual void Render(SDL_Renderer* renderer);

    void AddGameObject(const std::shared_ptr<GameObject>& gameObject);
    void RemoveGameObject(const std::shared_ptr<GameObject>& gameObject);

    std::vector<std::shared_ptr<GameObject>> GetGameObjectsByTag(const std::string& tag);
    std::shared_ptr<GameObject> GetGameObjectByTag(const std::string& tag);

    void CheckCollisions();

protected:

    struct PairHash {
        template <typename T1, typename T2>
        std::size_t operator()(const std::pair<T1, T2>& p) const {
            auto h1 = std::hash<T1>{}(p.first);
            auto h2 = std::hash<T2>{}(p.second);
            return h1 ^ (h2 << 1);
        }
    };


    virtual void OnCollision(GameObject* first, GameObject* second) {}

    std::vector<std::shared_ptr<GameObject>> gameObjects;
    std::unordered_map<std::string, std::vector<std::shared_ptr<GameObject>>> taggedObjects;
    std::unordered_set<std::pair<GameObject*, GameObject*>, PairHash> activeCollisions;

    // Helper method to maintain the tagged objects map
    void RegisterGameObjectTag(const std::shared_ptr<GameObject>& gameObject);
    void UnregisterGameObjectTag(const std::shared_ptr<GameObject>& gameObject);
};