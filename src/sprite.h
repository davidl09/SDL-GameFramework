#pragma once
#include <SDL2/SDL.h>
#include <memory>
#include "transform.h"

class Sprite {
public:
    Sprite(std::shared_ptr<SDL_Texture> texture, const SDL_Rect& srcRect);

    Sprite(std::shared_ptr<SDL_Texture> tex);

    void Render(SDL_Renderer* renderer, const Transform& transform);
    [[nodiscard]] const SDL_Rect& GetSourceRect() const;
    [[nodiscard]] SDL_Rect GetDestRect(const Transform& transform) const;
    
    // Animation support
    void SetFrame(int x, int y, int width, int height);
    void SetAlpha(Uint8 alpha);
    void SetBlendMode(SDL_BlendMode blendMode);
    
    [[nodiscard]] SDL_Texture* GetTexture() const { return texture.get(); }
    
private:
    std::shared_ptr<SDL_Texture> texture;
    SDL_Rect sourceRect;
};
