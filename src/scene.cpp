#include "scene.h"
#include <algorithm>

using namespace std;


void Scene::Update(float deltaTime) {
    // Update all game objects
    for (auto& obj : gameObjects) {
        obj->Update(deltaTime);
    }

    // Check for collisions
    CheckCollisions();
}

void Scene::Render(SDL_Renderer* renderer) {
    for (const auto& obj : gameObjects) {
        obj->Render(renderer);
    }
}

void Scene::AddGameObject(const std::shared_ptr<GameObject>& gameObject) {
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
    std::unordered_set<std::pair<GameObject*, GameObject*>, PairHash> currentFrameCollisions;

    // Check for collisions
    for (size_t i = 0; i < gameObjects.size(); ++i) {
        for (size_t j = i + 1; j < gameObjects.size(); ++j) {
            auto& first = gameObjects[i];
            auto& second = gameObjects[j];

            // Skip if either object is inactive
            if (!first->IsActive() || !second->IsActive()) {
                continue;
            }

            if (first->CheckCollision(*second)) {
                // Order the pair consistently
                GameObject* a = first.get();
                GameObject* b = second.get();
                if (a > b) {
                    swap(a, b);
                }
                auto collisionPair = std::make_pair(a, b);

                // Add to current frame's collisions
                currentFrameCollisions.insert(collisionPair);

                // If this is a new collision, call OnCollisionEnter
                if (activeCollisions.find(collisionPair) == activeCollisions.end()) {
                    first->OnCollisionEnter(second.get());
                    second->OnCollisionEnter(first.get());

                    // Call scene's collision handler
                    OnCollision(first.get(), second.get());
                }
            }
        }
    }

    // Check for collision exits
    for (const auto& pair : activeCollisions) {
        if (currentFrameCollisions.find(pair) == currentFrameCollisions.end()) {
            // This collision is no longer active
            pair.first->OnCollisionExit(pair.second);
            pair.second->OnCollisionExit(pair.first);
        }
    }

    // Update active collisions for next frame
    activeCollisions = std::move(currentFrameCollisions);
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