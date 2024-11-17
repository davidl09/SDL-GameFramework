#include "scene.h"
#include <algorithm>
#include <stdexcept>

Scene::~Scene() {
    // Clear collections in specific order to avoid dependency issues
    activeCollisions.clear();
    taggedObjects.clear();
    gameObjects.clear();
}

void Scene::Update(float deltaTime) {
    // Remove inactive or destroyed objects
    gameObjects.erase(
        std::remove_if(gameObjects.begin(), gameObjects.end(),
            [](const auto& obj) { return !obj || !obj->IsActive(); }),
        gameObjects.end()
    );

    // Clean up any expired tagged objects
    CleanupTags();

    // Update remaining objects
    for (const auto& obj : gameObjects) {
        if (obj && obj->IsActive()) {
            try {
                obj->Update(deltaTime);
            } catch (const std::exception& e) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                    "Error updating object: %s", e.what());
            }
        }
    }

    // Process collisions if not already doing so
    if (!isProcessingCollisions) {
        CheckCollisions();
    }
}

void Scene::Render(SDL_Renderer* renderer) {
    if (!renderer) return;

    // Render all active objects
    for (const auto& obj : gameObjects) {
        if (obj && obj->IsActive()) {
            try {
                obj->Render();
            } catch (const std::exception& e) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                    "Error rendering object: %s", e.what());
            }
        }
    }

    // Draw debug information if enabled
    if (debugDrawEnabled) {
        DrawDebugCollisions(renderer);
    }
}

void Scene::AddGameObject(const std::shared_ptr<GameObject>& gameObject) {
    if (!gameObject) {
        throw std::invalid_argument("Cannot add null GameObject");
    }

    gameObjects.push_back(gameObject);
    RegisterGameObjectTag(gameObject);
}

void Scene::RemoveGameObject(const std::shared_ptr<GameObject>& gameObject) {
    if (!gameObject) return;

    UnregisterGameObjectTag(gameObject);

    auto it = std::find(gameObjects.begin(), gameObjects.end(), gameObject);
    if (it != gameObjects.end()) {
        gameObjects.erase(it);
    }
}

std::vector<std::shared_ptr<GameObject>> Scene::GetGameObjectsByTag(const std::string& tag) {
    std::vector<std::shared_ptr<GameObject>> result;

    auto it = taggedObjects.find(tag);
    if (it != taggedObjects.end()) {
        result.reserve(it->second.size());
        for (const auto& info : it->second) {
            if (auto obj = info.object.lock()) {
                result.push_back(obj);
            }
        }
    }

    return result;
}

std::shared_ptr<GameObject> Scene::GetGameObjectByTag(const std::string& tag) {
    auto objects = GetGameObjectsByTag(tag);
    return objects.empty() ? nullptr : objects[0];
}

void Scene::CheckCollisions() {
    isProcessingCollisions = true;
    std::unordered_set<CollisionPair, WeakPtrPairHash, WeakPtrPairEqual> currentFrameCollisions;

    try {
        // Check for new collisions
        for (size_t i = 0; i < gameObjects.size(); ++i) {
            for (size_t j = i + 1; j < gameObjects.size(); ++j) {
                ProcessCollisionPair(gameObjects[i], gameObjects[j], currentFrameCollisions);
            }
        }

        // Process collision exits
        ProcessCollisionExits(currentFrameCollisions);

        // Update active collisions for next frame
        activeCollisions = std::move(currentFrameCollisions);
    }
    catch (...) {
        isProcessingCollisions = false;
        throw;
    }

    isProcessingCollisions = false;
}

void Scene::ProcessCollisionPair(
    const std::shared_ptr<GameObject>& first,
    const std::shared_ptr<GameObject>& second,
    std::unordered_set<CollisionPair, WeakPtrPairHash, WeakPtrPairEqual>& currentCollisions)
{
    if (!first->IsActive() || !second->IsActive()) {
        return;
    }

    if (first->CheckCollision(*second)) {
        CollisionPair pair = CreateOrderedPair(first, second);
        currentCollisions.insert(pair);

        if (activeCollisions.find(pair) == activeCollisions.end()) {
            SafeCallCollisionHandlers(first, second);
        }
    }
}

void Scene::ProcessCollisionExits(
    const std::unordered_set<CollisionPair, WeakPtrPairHash, WeakPtrPairEqual>& currentCollisions)
{
    for (const auto& pair : activeCollisions) {
        if (auto first = pair.first.lock()) {
            if (auto second = pair.second.lock()) {
                if (currentCollisions.find(pair) == currentCollisions.end()) {
                    SafeCallCollisionExitHandlers(first, second);
                }
            }
        }
    }
}

void Scene::SafeCallCollisionHandlers(
    const std::shared_ptr<GameObject>& first,
    const std::shared_ptr<GameObject>& second)
{
    try {
        first->OnCollisionEnter(second.get());
        second->OnCollisionEnter(first.get());
        OnCollision(first.get(), second.get());
    }
    catch (const std::exception& e) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "Error in collision handlers: %s", e.what());
    }
}

void Scene::SafeCallCollisionExitHandlers(
    const std::shared_ptr<GameObject>& first,
    const std::shared_ptr<GameObject>& second)
{
    try {
        first->OnCollisionExit(second.get());
        second->OnCollisionExit(first.get());
    }
    catch (const std::exception& e) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "Error in collision exit handlers: %s", e.what());
    }
}

Scene::CollisionPair Scene::CreateOrderedPair(
    const std::shared_ptr<GameObject>& first,
    const std::shared_ptr<GameObject>& second)
{
    return first.get() < second.get()
        ? CollisionPair(first, second)
        : CollisionPair(second, first);
}

void Scene::RegisterGameObjectTag(const std::shared_ptr<GameObject>& gameObject) {
    const std::string& tag = gameObject->GetTag();
    if (!tag.empty()) {
        TaggedObjectInfo info{
            std::weak_ptr<GameObject>(gameObject),
            gameObjects.size() - 1  // Index in gameObjects vector
        };
        taggedObjects[tag].push_back(info);
    }
}

void Scene::UnregisterGameObjectTag(const std::shared_ptr<GameObject>& gameObject) {
    const std::string& tag = gameObject->GetTag();
    if (!tag.empty()) {
        auto it = taggedObjects.find(tag);
        if (it != taggedObjects.end()) {
            auto& tagList = it->second;
            tagList.erase(
                std::remove_if(tagList.begin(), tagList.end(),
                    [gameObject](const TaggedObjectInfo& info) {
                        auto obj = info.object.lock();
                        return !obj || obj == gameObject;
                    }),
                tagList.end()
            );

            if (tagList.empty()) {
                taggedObjects.erase(it);
            }
        }
    }
}

void Scene::CleanupTags() {
    for (auto it = taggedObjects.begin(); it != taggedObjects.end();) {
        auto& tagList = it->second;
        tagList.erase(
            std::remove_if(tagList.begin(), tagList.end(),
                [](const TaggedObjectInfo& info) {
                    return info.object.expired();
                }),
            tagList.end()
        );

        if (tagList.empty()) {
            it = taggedObjects.erase(it);
        } else {
            ++it;
        }
    }
}

void Scene::DrawDebugCollisions(SDL_Renderer* renderer) {
    if (!renderer) return;

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    // Draw lines between actively colliding objects
    for (const auto& pair : activeCollisions) {
        auto first = pair.first.lock();
        auto second = pair.second.lock();

        if (first && second) {
            const auto& pos1 = first->GetTransform().position;
            const auto& pos2 = second->GetTransform().position;

            SDL_RenderDrawLine(renderer,
                static_cast<int>(pos1.x),
                static_cast<int>(pos1.y),
                static_cast<int>(pos2.x),
                static_cast<int>(pos2.y)
            );
        }
    }
}