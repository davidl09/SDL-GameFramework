#pragma once
#include <SDL2/SDL.h>
#include <memory>
#include "Transform.h"

class Sprite {
public:
    Sprite(std::shared_ptr<SDL_Texture> texture, const SDL_Rect& srcRect);
    
    void Render(SDL_Renderer* renderer, const Transform& transform);
    const SDL_Rect& GetSourceRect() const;
    SDL_Rect GetDestRect(const Transform& transform) const;
    
    // Animation support
    void SetFrame(int x, int y, int width, int height);
    void SetAlpha(Uint8 alpha);
    void SetBlendMode(SDL_BlendMode blendMode);
    
    SDL_Texture* GetTexture() const { return texture.get(); }
    
private:
    std::shared_ptr<SDL_Texture> texture;
    SDL_Rect sourceRect;
};
