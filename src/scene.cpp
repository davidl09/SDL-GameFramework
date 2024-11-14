#include "scene.h"
#include <algorithm>

void Scene::Update(float deltaTime) {
    // Update all game objects
    for (auto& obj : gameObjects) {
        obj->Update(deltaTime);
    }

    // Check for collisions
    CheckCollisions();
}

void Scene::Render(SDL_Renderer* renderer) {
    for (auto& obj : gameObjects) {
        obj->Render(renderer);
    }
}

void Scene::AddGameObject(std::shared_ptr<GameObject> gameObject) {
    gameObjects.push_back(gameObject);
    RegisterGameObjectTag(gameObject);
}

void Scene::RemoveGameObject(const std::shared_ptr<GameObject>& gameObject) {
    UnregisterGameObjectTag(gameObject);

    auto it = std::find(gameObjects.begin(), gameObjects.end(), gameObject);
    if (it != gameObjects.end()) {
        gameObjects.erase(it);
    }
}

std::vector<std::shared_ptr<GameObject>> Scene::GetGameObjectsByTag(const std::string& tag) {
    auto it = taggedObjects.find(tag);
    if (it != taggedObjects.end()) {
        return it->second;
    }
    return std::vector<std::shared_ptr<GameObject>>();
}

std::shared_ptr<GameObject> Scene::GetGameObjectByTag(const std::string& tag) {
    auto objects = GetGameObjectsByTag(tag);
    if (!objects.empty()) {
        return objects[0];
    }
    return nullptr;
}

void Scene::CheckCollisions() {
    for (size_t i = 0; i < gameObjects.size(); ++i) {
        for (size_t j = i + 1; j < gameObjects.size(); ++j) {
            if (gameObjects[i]->CheckCollision(*gameObjects[j])) {
                OnCollision(gameObjects[i].get(), gameObjects[j].get());
            }
        }
    }
}

void Scene::RegisterGameObjectTag(const std::shared_ptr<GameObject>& gameObject) {
    const std::string& tag = gameObject->GetTag();
    if (!tag.empty()) {
        taggedObjects[tag].push_back(gameObject);
    }
}

void Scene::UnregisterGameObjectTag(const std::shared_ptr<GameObject>& gameObject) {
    const std::string& tag = gameObject->GetTag();
    if (!tag.empty()) {
        auto& taggedList = taggedObjects[tag];
        auto it = std::find(taggedList.begin(), taggedList.end(), gameObject);
        if (it != taggedList.end()) {
            taggedList.erase(it);
        }
    }
}