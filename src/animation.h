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
    struct Frame {
        SDL_Rect sourceRect;
        float duration;
    };

    // Default constructor
    Animation() : name(""), looping(true), currentFrame(0), 
                 elapsedTime(0.0f), isPlaying(false) {}

    Animation(const std::string& name, bool looping = true)
        : name(name), looping(looping), currentFrame(0),
          elapsedTime(0.0f), isPlaying(false) {}

    void AddFrame(const SDL_Rect& rect, float duration) {
        frames.push_back({rect, duration});
    }

    void Play() {
        isPlaying = true;
        currentFrame = 0;
        elapsedTime = 0.0f;
    }

    void Pause() { isPlaying = false; }
    void Resume() { isPlaying = true; }
    void Stop() {
        isPlaying = false;
        currentFrame = 0;
        elapsedTime = 0.0f;
    }

    void Update(float deltaTime) {
        if (!isPlaying || frames.empty()) return;

        elapsedTime += deltaTime;
        if (elapsedTime >= frames[currentFrame].duration) {
            elapsedTime -= frames[currentFrame].duration;
            
            if (currentFrame + 1 >= frames.size()) {
                if (looping) {
                    currentFrame = 0;
                } else {
                    isPlaying = false;
                    elapsedTime = 0.0f;
                }
            } else {
                currentFrame++;
            }
        }
    }

    const Frame& GetCurrentFrame() const {
        return frames[currentFrame];
    }

    bool IsPlaying() const { return isPlaying; }
    bool IsLooping() const { return looping; }
    const std::string& GetName() const { return name; }

private:
    std::string name;
    std::vector<Frame> frames;
    bool looping;
    size_t currentFrame;
    float elapsedTime;
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
