#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <memory>
#include <string>
#include <vector>
#include "../vector2d.h"

enum class UIAnchor {
    TopLeft,
    Top,
    TopRight,
    Left,
    Center,
    Right,
    BottomLeft,
    Bottom,
    BottomRight
};

class UIElement {
public:
    UIElement() : parent(nullptr), active(true), visible(true),
                 anchor(UIAnchor::TopLeft), position(0, 0),
                 size(100, 30), scale(1, 1) {}
    
    virtual ~UIElement() = default;

    virtual void Update(float deltaTime) {
        if (!active) return;
        
        for (auto& child : children) {
            child->Update(deltaTime);
        }
    }

    virtual void Render(SDL_Renderer* renderer) {
        if (!visible) return;
        
        for (auto& child : children) {
            child->Render(renderer);
        }
    }

    void AddChild(std::shared_ptr<UIElement> child) {
        child->parent = this;
        children.push_back(child);
    }

    void RemoveChild(std::shared_ptr<UIElement> child) {
        auto it = std::find(children.begin(), children.end(), child);
        if (it != children.end()) {
            (*it)->parent = nullptr;
            children.erase(it);
        }
    }

    Vector2D GetWorldPosition() const {
        Vector2D worldPos = position;
        if (parent) {
            worldPos += parent->GetWorldPosition();
        }
        return worldPos;
    }

    virtual SDL_Rect GetBounds() const {
        Vector2D worldPos = GetWorldPosition();
        return {
            static_cast<int>(worldPos.x),
            static_cast<int>(worldPos.y),
            static_cast<int>(size.x * scale.x),
            static_cast<int>(size.y * scale.y)
        };
    }

    // Setters
    void SetPosition(const Vector2D& pos) { position = pos; }
    void SetSize(const Vector2D& newSize) { size = newSize; }
    void SetScale(const Vector2D& newScale) { scale = newScale; }
    void SetAnchor(UIAnchor newAnchor) { anchor = newAnchor; }
    void SetActive(bool isActive) { active = isActive; }
    void SetVisible(bool isVisible) { visible = isVisible; }

    // Getters
    const Vector2D& GetPosition() const { return position; }
    const Vector2D& GetSize() const { return size; }
    const Vector2D& GetScale() const { return scale; }
    UIAnchor GetAnchor() const { return anchor; }
    bool IsActive() const { return active; }
    bool IsVisible() const { return visible; }
    UIElement* GetParent() const { return parent; }
    const std::vector<std::shared_ptr<UIElement>>& GetChildren() const { return children; }

protected:
    UIElement* parent;
    std::vector<std::shared_ptr<UIElement>> children;
    bool active;
    bool visible;
    UIAnchor anchor;
    Vector2D position;
    Vector2D size;
    Vector2D scale;
};
