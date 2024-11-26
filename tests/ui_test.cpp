#include <gtest/gtest.h>
#include "ui/ui_element.h"
#include "ui/ui_button.h"
#include "ui/ui_text.h"

class UITest : public ::testing::Test {
protected:
    void SetUp() override {
        element = std::make_shared<UIElement>();
    }

    std::shared_ptr<UIElement> element;
};

TEST_F(UITest, UIElementInitialState) {
    EXPECT_TRUE(element->IsActive());
    EXPECT_TRUE(element->IsVisible());
    EXPECT_EQ(element->GetAnchor(), UIAnchor::TopLeft);
    EXPECT_EQ(element->GetPosition(), Vector2D(0, 0));
    EXPECT_EQ(element->GetScale(), Vector2D(1, 1));
}

TEST_F(UITest, UIElementSetters) {
    element->SetPosition(Vector2D(100, 100));
    element->SetSize(Vector2D(200, 50));
    element->SetScale(Vector2D(2, 2));
    element->SetAnchor(UIAnchor::Center);
    element->SetActive(false);
    element->SetVisible(false);

    EXPECT_EQ(element->GetPosition(), Vector2D(100, 100));
    EXPECT_EQ(element->GetSize(), Vector2D(200, 50));
    EXPECT_EQ(element->GetScale(), Vector2D(2, 2));
    EXPECT_EQ(element->GetAnchor(), UIAnchor::Center);
    EXPECT_FALSE(element->IsActive());
    EXPECT_FALSE(element->IsVisible());
}

TEST_F(UITest, UIElementHierarchy) {
    auto child1 = std::make_shared<UIElement>();
    auto child2 = std::make_shared<UIElement>();
    
    element->AddChild(child1);
    element->AddChild(child2);
    
    EXPECT_EQ(element->GetChildren().size(), 2);
    EXPECT_EQ(child1->GetParent(), element.get());
    EXPECT_EQ(child2->GetParent(), element.get());
    
    element->RemoveChild(child1);
    EXPECT_EQ(element->GetChildren().size(), 1);
    EXPECT_EQ(child1->GetParent(), nullptr);
}

TEST_F(UITest, UIElementWorldPosition) {
    auto parent = std::make_shared<UIElement>();
    auto child = std::make_shared<UIElement>();
    
    parent->SetPosition(Vector2D(100, 100));
    child->SetPosition(Vector2D(50, 50));
    
    parent->AddChild(child);
    
    Vector2D worldPos = child->GetWorldPosition();
    EXPECT_EQ(worldPos, Vector2D(150, 150));
}

TEST_F(UITest, UIButtonStates) {
    auto button = std::make_shared<UIButton>();
    bool callbackCalled = false;
    
    button->SetCallback([&callbackCalled]() {
        callbackCalled = true;
    });
    
    // Simulate mouse over button
    SDL_Rect bounds = button->GetBounds();
    Vector2D mousePos(bounds.x + bounds.w/2, bounds.y + bounds.h/2);
    // Note: In a real test, you'd need to mock Mouse::GetPosition()
    
    button->Update(0.016f); // One frame at 60fps
    
    // Test button visual states
    SDL_Color normalColor = {200, 200, 200, 255};
    SDL_Color hoverColor = {220, 220, 220, 255};
    SDL_Color pressedColor = {180, 180, 180, 255};
    
    button->SetNormalColor(normalColor);
    button->SetHoverColor(hoverColor);
    button->SetPressedColor(pressedColor);
    
    // Note: In a real test, you'd verify the actual rendering
}

TEST_F(UITest, UITextProperties) {
    auto text = std::make_shared<UIText>();
    
    text->SetText("Test Text");
    text->SetColor({255, 0, 0, 255});
    text->SetWrapWidth(200);
    text->SetAlignment(UIText::TextAlignment::Center);
    
    // Note: Full text rendering tests would require a mock SDL_TTF context
}

TEST_F(UITest, UIElementBounds) {
    element->SetPosition(Vector2D(100, 100));
    element->SetSize(Vector2D(200, 50));
    element->SetScale(Vector2D(2, 2));
    
    SDL_Rect bounds = element->GetBounds();
    EXPECT_EQ(bounds.x, 100);
    EXPECT_EQ(bounds.y, 100);
    EXPECT_EQ(bounds.w, 400); // 200 * 2
    EXPECT_EQ(bounds.h, 100); // 50 * 2
}

TEST_F(UITest, UIElementVisibilityPropagation) {
    auto parent = std::make_shared<UIElement>();
    auto child = std::make_shared<UIElement>();
    
    parent->AddChild(child);
    parent->SetVisible(false);
    
    // Child should still report as visible (visibility is checked during render)
    EXPECT_TRUE(child->IsVisible());
    
    // Note: In a real test, you'd verify that the child is not rendered
    // when the parent is invisible by mocking the renderer
}
