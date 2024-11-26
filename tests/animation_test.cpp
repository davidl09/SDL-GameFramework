#include <gtest/gtest.h>
#include "animation.h"

class AnimationTest : public ::testing::Test {
protected:
    void SetUp() override {
        animation = std::make_unique<Animation>("test", true);
    }

    std::unique_ptr<Animation> animation;
};

TEST_F(AnimationTest, InitialState) {
    EXPECT_TRUE(animation != nullptr);
    EXPECT_TRUE(animation->IsLooping());
    EXPECT_FALSE(animation->IsPlaying());
}

TEST_F(AnimationTest, AddFrames) {
    SDL_Rect frame1 = {0, 0, 32, 32};
    SDL_Rect frame2 = {32, 0, 32, 32};
    float frameDuration = 0.1f;
    
    animation->AddFrame(frame1, frameDuration);
    animation->AddFrame(frame2, frameDuration);
    
    const auto& currentFrame = animation->GetCurrentFrame();
    EXPECT_EQ(currentFrame.sourceRect.x, frame1.x);
    EXPECT_EQ(currentFrame.sourceRect.y, frame1.y);
    EXPECT_EQ(currentFrame.sourceRect.w, frame1.w);
    EXPECT_EQ(currentFrame.sourceRect.h, frame1.h);
    EXPECT_EQ(currentFrame.duration, frameDuration);
}

TEST_F(AnimationTest, PlaybackControl) {
    SDL_Rect frame = {0, 0, 32, 32};
    animation->AddFrame(frame, 0.1f);
    
    EXPECT_FALSE(animation->IsPlaying());
    animation->Play();
    EXPECT_TRUE(animation->IsPlaying());
    animation->Stop();
    EXPECT_FALSE(animation->IsPlaying());
    animation->Play();
    EXPECT_TRUE(animation->IsPlaying());
    animation->Pause();
    EXPECT_FALSE(animation->IsPlaying());
}

TEST_F(AnimationTest, FrameProgression) {
    SDL_Rect frame1 = {0, 0, 32, 32};
    SDL_Rect frame2 = {32, 0, 32, 32};
    float frameDuration = 0.1f;
    
    animation->AddFrame(frame1, frameDuration);
    animation->AddFrame(frame2, frameDuration);
    animation->Play();
    
    // First frame
    EXPECT_EQ(animation->GetCurrentFrame().sourceRect.x, frame1.x);
    
    // Update halfway through first frame
    animation->Update(frameDuration / 2);
    EXPECT_EQ(animation->GetCurrentFrame().sourceRect.x, frame1.x);
    
    // Update to second frame
    animation->Update(frameDuration);
    EXPECT_EQ(animation->GetCurrentFrame().sourceRect.x, frame2.x);
    
    // Update past second frame
    animation->Update(frameDuration + 0.01f);
    EXPECT_EQ(animation->GetCurrentFrame().sourceRect.x, frame1.x); // Back to first frame (looping)
}

TEST_F(AnimationTest, NonLoopingBehavior) {
    animation = std::make_unique<Animation>("test", false); // Non-looping
    
    SDL_Rect frame1 = {0, 0, 32, 32};
    SDL_Rect frame2 = {32, 0, 32, 32};
    float frameDuration = 0.1f;
    
    animation->AddFrame(frame1, frameDuration);
    animation->AddFrame(frame2, frameDuration);
    animation->Play();
    
    // Update to second frame
    animation->Update(frameDuration);
    EXPECT_TRUE(animation->IsPlaying());
    EXPECT_EQ(animation->GetCurrentFrame().sourceRect.x, frame2.x);
    
    // Update to complete second frame
    animation->Update(frameDuration);
    EXPECT_FALSE(animation->IsPlaying());
    EXPECT_EQ(animation->GetCurrentFrame().sourceRect.x, frame2.x);
}
