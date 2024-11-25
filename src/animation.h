#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <SDL2/SDL.h>
#include "vector2d.h"

/**
 * @brief Represents a single animation frame
 */
struct AnimationFrame {
    SDL_Rect sourceRect;  // Source rectangle in the sprite sheet
    float duration;       // Duration of this frame in seconds
    Vector2D pivot;       // Pivot point for rotation (normalized coordinates)
};

/**
 * @brief Represents a complete animation sequence
 */
class Animation {
public:
    Animation(const std::string& name, bool looping = true)
        : name(name), looping(looping), currentFrame(0), 
          timeAccumulator(0.0f), isPlaying(false) {}

    void AddFrame(const SDL_Rect& rect, float duration = 0.1f, const Vector2D& pivot = Vector2D(0.5f, 0.5f)) {
        frames.push_back({rect, duration, pivot});
    }

    const std::string& GetName() const { return name; }
    bool IsLooping() const { return looping; }
    bool IsPlaying() const { return isPlaying; }
    const AnimationFrame& GetCurrentFrame() const { return frames[currentFrame]; }

    void Play() {
        isPlaying = true;
        currentFrame = 0;
        timeAccumulator = 0.0f;
    }

    void Stop() {
        isPlaying = false;
        currentFrame = 0;
        timeAccumulator = 0.0f;
    }

    void Pause() {
        isPlaying = false;
    }

    void Resume() {
        isPlaying = true;
    }

    void Update(float deltaTime) {
        if (!isPlaying || frames.empty()) return;

        timeAccumulator += deltaTime;
        while (timeAccumulator >= frames[currentFrame].duration) {
            timeAccumulator -= frames[currentFrame].duration;
            currentFrame++;

            if (currentFrame >= frames.size()) {
                if (looping) {
                    currentFrame = 0;
                } else {
                    currentFrame = frames.size() - 1;
                    Stop();
                    break;
                }
            }
        }
    }

private:
    std::string name;
    std::vector<AnimationFrame> frames;
    bool looping;
    size_t currentFrame;
    float timeAccumulator;
    bool isPlaying;
};

/**
 * @brief Component that manages sprite animations
 */
class AnimationController {
public:
    void AddAnimation(const std::string& name, bool looping = true) {
        animations[name] = Animation(name, looping);
    }

    Animation* GetAnimation(const std::string& name) {
        auto it = animations.find(name);
        return it != animations.end() ? &it->second : nullptr;
    }

    void Play(const std::string& name) {
        if (currentAnimation && currentAnimation->GetName() == name) return;
        
        auto* anim = GetAnimation(name);
        if (anim) {
            if (currentAnimation) {
                currentAnimation->Stop();
            }
            currentAnimation = anim;
            currentAnimation->Play();
        }
    }

    void Stop() {
        if (currentAnimation) {
            currentAnimation->Stop();
            currentAnimation = nullptr;
        }
    }

    void Update(float deltaTime) {
        if (currentAnimation) {
            currentAnimation->Update(deltaTime);
        }
    }

    const Animation* GetCurrentAnimation() const {
        return currentAnimation;
    }

private:
    std::unordered_map<std::string, Animation> animations;
    Animation* currentAnimation = nullptr;
};
