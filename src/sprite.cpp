#include "sprite.h"

#include <utility>

Sprite::Sprite(std::shared_ptr<SDL_Texture> texture, const SDL_Rect& srcRect)
    : texture(std::move(texture)), sourceRect(srcRect) {}

Sprite::Sprite(std::shared_ptr<SDL_Texture> tex)
    : texture(std::move(tex)), sourceRect(SDL_Rect()) {
    // Get full texture dimensions
    int width, height;
    SDL_QueryTexture(texture.get(), nullptr, nullptr, &width, &height);

    // Set source rectangle to full texture
    sourceRect = {0, 0, width, height};
}

void Sprite::Render(SDL_Renderer* renderer, const Transform& transform) {
    SDL_Rect destRect = GetDestRect(transform);
    
    // Calculate the pivot point for rotation (center of the sprite)
    SDL_Point center = {
        destRect.w / 2,
        destRect.h / 2
    };
    
    // Adjust destination rectangle to rotate around center
    destRect.x -= center.x;
    destRect.y -= center.y;
    
    SDL_RenderCopyEx(
        renderer,
        texture.get(),
        &sourceRect,
        &destRect,
        transform.rotation,
        &center,
        SDL_FLIP_NONE
    );
}

const SDL_Rect& Sprite::GetSourceRect() const {
    return sourceRect;
}

SDL_Rect Sprite::GetDestRect(const Transform& transform) const {
    SDL_Rect destRect;
    destRect.x = static_cast<int>(transform.position.x);
    destRect.y = static_cast<int>(transform.position.y);
    destRect.w = static_cast<int>(sourceRect.w * transform.scale.x);
    destRect.h = static_cast<int>(sourceRect.h * transform.scale.y);
    return destRect;
}

void Sprite::SetFrame(int x, int y, int width, int height) {
    sourceRect.x = x;
    sourceRect.y = y;
    sourceRect.w = width;
    sourceRect.h = height;
}

void Sprite::SetAlpha(Uint8 alpha) {
    SDL_SetTextureAlphaMod(texture.get(), alpha);
}

void Sprite::SetBlendMode(SDL_BlendMode blendMode) {
    SDL_SetTextureBlendMode(texture.get(), blendMode);
}
