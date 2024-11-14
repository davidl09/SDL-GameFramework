#pragma once
#include <vector>
#include <memory>
#include <unordered_map>
#include <string>
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
    virtual void OnCollision(GameObject* first, GameObject* second) {}

    std::vector<std::shared_ptr<GameObject>> gameObjects;
    std::unordered_map<std::string, std::vector<std::shared_ptr<GameObject>>> taggedObjects;

    // Helper method to maintain the tagged objects map
    void RegisterGameObjectTag(const std::shared_ptr<GameObject>& gameObject);
    void UnregisterGameObjectTag(const std::shared_ptr<GameObject>& gameObject);
};