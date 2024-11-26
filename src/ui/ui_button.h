#pragma once
#include "ui_element.h"
#include "../mouse.h"
#include <functional>

class UIButton : public UIElement {
public:
    UIButton() : UIElement(), 
                normalColor{200, 200, 200, 255},
                hoverColor{220, 220, 220, 255},
                pressedColor{180, 180, 180, 255},
                textColor{0, 0, 0, 255},
                isHovered(false),
                isPressed(false) {}

    void SetText(const std::string& text) { this->text = text; }
    void SetFont(TTF_Font* font) { this->font = font; }
    void SetCallback(std::function<void()> callback) { onClick = callback; }
    
    void SetNormalColor(const SDL_Color& color) { normalColor = color; }
    void SetHoverColor(const SDL_Color& color) { hoverColor = color; }
    void SetPressedColor(const SDL_Color& color) { pressedColor = color; }
    void SetTextColor(const SDL_Color& color) { textColor = color; }

    void Update(float deltaTime) override {
        if (!active) return;
        UIElement::Update(deltaTime);

        auto& mouse = Mouse::Instance();
        Vector2D mousePos = mouse.GetPosition();
        SDL_Rect bounds = GetBounds();
        
        bool wasPressed = isPressed;
        isHovered = mousePos.x >= bounds.x && mousePos.x < bounds.x + bounds.w &&
                    mousePos.y >= bounds.y && mousePos.y < bounds.y + bounds.h;
        
        isPressed = isHovered && mouse.IsButtonPressed(SDL_BUTTON_LEFT);

        // Trigger callback on release
        if (wasPressed && !isPressed && isHovered && onClick) {
            onClick();
        }
    }

    void Render(SDL_Renderer* renderer) override {
        if (!visible) return;

        SDL_Rect bounds = GetBounds();
        
        // Draw button background
        SDL_Color color = normalColor;
        if (isPressed) color = pressedColor;
        else if (isHovered) color = hoverColor;

        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(renderer, &bounds);

        // Draw border
        SDL_SetRenderDrawColor(renderer, color.r * 0.8f, color.g * 0.8f, color.b * 0.8f, color.a);
        SDL_RenderDrawRect(renderer, &bounds);

        // Draw text if font is set
        if (font && !text.empty()) {
            SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), textColor);
            if (surface) {
                SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                SDL_FreeSurface(surface);

                if (texture) {
                    int textWidth, textHeight;
                    SDL_QueryTexture(texture, nullptr, nullptr, &textWidth, &textHeight);
                    
                    SDL_Rect textRect = {
                        bounds.x + (bounds.w - textWidth) / 2,
                        bounds.y + (bounds.h - textHeight) / 2,
                        textWidth,
                        textHeight
                    };
                    
                    SDL_RenderCopy(renderer, texture, nullptr, &textRect);
                    SDL_DestroyTexture(texture);
                }
            }
        }

        UIElement::Render(renderer);
    }

private:
    std::string text;
    TTF_Font* font = nullptr;
    std::function<void()> onClick;
    
    SDL_Color normalColor;
    SDL_Color hoverColor;
    SDL_Color pressedColor;
    SDL_Color textColor;
    
    bool isHovered;
    bool isPressed;
};
