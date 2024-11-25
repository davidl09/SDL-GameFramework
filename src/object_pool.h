#pragma once
#include <vector>
#include <queue>
#include <memory>
#include <functional>

/**
 * @brief Generic object pool for efficient object reuse
 * @tparam T Type of object to pool
 */
template<typename T>
class ObjectPool {
public:
    /**
     * @brief Create a new object pool
     * @param initialSize Initial number of objects to create
     * @param growSize Number of objects to create when pool is empty
     * @param initializer Function to initialize new objects
     */
    ObjectPool(size_t initialSize = 100, size_t growSize = 50,
               std::function<void(T*)> initializer = nullptr)
        : growSize(growSize), initializer(initializer) {
        Grow(initialSize);
    }

    /**
     * @brief Get an object from the pool
     * @return Shared pointer to the object
     */
    std::shared_ptr<T> Acquire() {
        if (available.empty()) {
            Grow(growSize);
        }

        auto obj = available.front();
        available.pop();
        
        // Create deleter that returns object to pool
        return std::shared_ptr<T>(obj.get(), [this](T* ptr) {
            available.push(std::unique_ptr<T>(ptr));
        });
    }

    /**
     * @brief Get current number of available objects
     * @return Number of available objects
     */
    size_t GetAvailableCount() const {
        return available.size();
    }

    /**
     * @brief Get total number of objects in pool
     * @return Total number of objects
     */
    size_t GetTotalSize() const {
        return allObjects.size();
    }

    /**
     * @brief Clear the pool and destroy all objects
     */
    void Clear() {
        while (!available.empty()) {
            available.pop();
        }
        allObjects.clear();
    }

private:
    /**
     * @brief Grow the pool by creating new objects
     * @param count Number of objects to create
     */
    void Grow(size_t count) {
        for (size_t i = 0; i < count; ++i) {
            auto obj = std::make_unique<T>();
            if (initializer) {
                initializer(obj.get());
            }
            available.push(std::move(obj));
        }
    }

    size_t growSize;
    std::function<void(T*)> initializer;
    std::queue<std::unique_ptr<T>> available;
    std::vector<std::weak_ptr<T>> allObjects;  // For tracking
};
