#pragma once
#include "ui_element.h"

class UIText : public UIElement {
public:
    UIText() : UIElement(),
              color{0, 0, 0, 255},
              wrapWidth(0),
              alignment(TextAlignment::Left) {}

    enum class TextAlignment {
        Left,
        Center,
        Right
    };

    void SetText(const std::string& text) { 
        if (this->text != text) {
            this->text = text;
            UpdateTexture();
        }
    }

    void SetFont(TTF_Font* font) { 
        this->font = font;
        UpdateTexture();
    }

    void SetColor(const SDL_Color& color) { 
        this->color = color;
        UpdateTexture();
    }

    void SetWrapWidth(int width) {
        wrapWidth = width;
        UpdateTexture();
    }

    void SetAlignment(TextAlignment align) {
        alignment = align;
        UpdateTexture();
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

    ~UIText() {
        if (texture) {
            SDL_DestroyTexture(texture);
        }
    }

private:
    void UpdateTexture() {
        if (!font || text.empty()) return;

        if (texture) {
            SDL_DestroyTexture(texture);
            texture = nullptr;
        }

        SDL_Surface* surface;
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
    }

    std::string text;
    TTF_Font* font = nullptr;
    SDL_Texture* texture = nullptr;
    SDL_Color color;
    int wrapWidth;
    int textWidth = 0;
    int textHeight = 0;
    TextAlignment alignment;
};
