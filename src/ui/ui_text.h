#pragma once
#include "ui_element.h"
#include "../game.h"
#include <SDL_ttf.h>
#include <memory>
#include <string>

class UIText : public UIElement {
public:
    enum class TextAlignment {
        Left,
        Center,
        Right
    };

    UIText() : UIElement(),
        text(""),
        color({255, 255, 255, 255}),
        wrapWidth(0),
        alignment(TextAlignment::Left),
        texture(nullptr),
        dirty(true) {}

    ~UIText() {
        if (texture) {
            SDL_DestroyTexture(texture);
        }
    }

    void SetText(const std::string& newText) {
        if (text != newText) {
            text = newText;
            dirty = true;
        }
    }

    void SetColor(SDL_Color newColor) {
        if (color.r != newColor.r || color.g != newColor.g || 
            color.b != newColor.b || color.a != newColor.a) {
            color = newColor;
            dirty = true;
        }
    }

    void SetWrapWidth(int width) {
        if (wrapWidth != width) {
            wrapWidth = width;
            dirty = true;
        }
    }

    void SetAlignment(TextAlignment newAlignment) {
        if (alignment != newAlignment) {
            alignment = newAlignment;
            dirty = true;
        }
    }

    void Update(float deltaTime) override {
        if (dirty) {
            UpdateTexture();
        }
    }

    void Render(SDL_Renderer* renderer) override {
        if (!visible || !texture) return;

        SDL_Rect bounds = GetBounds();
        int xPos = bounds.x;

        // Adjust position based on alignment
        if (alignment != TextAlignment::Left && textWidth < bounds.w) {
            if (alignment == TextAlignment::Center) {
                xPos += (bounds.w - textWidth) / 2;
            } else if (alignment == TextAlignment::Right) {
                xPos += bounds.w - textWidth;
            }
        }

        SDL_Rect destRect = {
            xPos,
            bounds.y + (bounds.h - textHeight) / 2,
            textWidth,
            textHeight
        };

        SDL_RenderCopy(renderer, texture, nullptr, &destRect);
        UIElement::Render(renderer);
    }

private:
    void UpdateTexture() {
        if (texture) {
            SDL_DestroyTexture(texture);
            texture = nullptr;
        }

        TTF_Font* font = TTF_OpenFont("assets/fonts/default.ttf", 16);
        if (!font) {
            // Handle error
            return;
        }

        SDL_Surface* surface = nullptr;
        if (wrapWidth > 0) {
            surface = TTF_RenderText_Blended_Wrapped(font, text.c_str(), color, wrapWidth);
        } else {
            surface = TTF_RenderText_Blended(font, text.c_str(), color);
        }

        if (surface) {
            texture = SDL_CreateTextureFromSurface(Game::Instance().GetRenderer(), surface);
            textWidth = surface->w;
            textHeight = surface->h;
            SDL_FreeSurface(surface);
        }

        TTF_CloseFont(font);
        dirty = false;
    }

    std::string text;
    SDL_Color color;
    int wrapWidth;
    TextAlignment alignment;
    SDL_Texture* texture;
    bool dirty;
    int textWidth = 0;
    int textHeight = 0;
};
