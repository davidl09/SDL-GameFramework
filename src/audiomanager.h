//
// Created by David Laeer on 2024-11-16.
//
#pragma once
#include <SDL_mixer.h>
#include <string>
#include <map>

class AudioManager {
public:
    // Configuration for sound playback
    struct PlayConfig {
        int loops = 0;         // 0 = play once, -1 = infinite loop, >0 = play n+1 times
        int volume = 128;      // 0-128
        int fadeInMs = 0;      // Fade in time in milliseconds
        int fadeOutMs = 0;     // Fade out time in milliseconds
        int channel = -1;      // Specific channel to play on (-1 for auto-assign)
    };

    static AudioManager& Instance();

    // Delete copy constructor and assignment operator
    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;

    // Sound effects control
    int PlaySound(const std::string& path, const PlayConfig& config);
    int PlayLoopedSound(const std::string& path, int volume = 128); // Returns channel ID
    void StopSound(int channel, int fadeOutMs = 0);
    void StopSound(const std::string& path, int fadeOutMs = 0);
    void SetSoundVolume(int channel, int volume); // 0-128
    bool IsSoundPlaying(int channel) const;
    void PauseSound(int channel);
    void ResumeSound(int channel);

    // Music control
    void PlayMusic(const std::string& path, const PlayConfig& config);
    void StopMusic(int fadeOutMs = 0);
    void PauseMusic();
    void ResumeMusic();
    void SetMusicVolume(int volume); // 0-128
    bool IsMusicPlaying() const;
    bool IsMusicPaused() const;

    // Find channel by sound
    int FindChannel(const std::string& path) const;

    // Global audio control
    void StopAll(int fadeOutMs = 0);
    void PauseAll();
    void ResumeAll();
    void SetMasterVolume(int volume); // 0-128
    void EnableSound(bool enabled);
    bool IsSoundEnabled() const { return soundEnabled; }

    // Channel reservation
    int ReserveChannels(int num);  // Returns number of channels reserved
    void UnreserveChannels(int num);

    // Get status
    [[nodiscard]] int GetMasterVolume() const { return masterVolume; }
    [[nodiscard]] int GetMusicVolume() const;
    [[nodiscard]] const std::string& GetCurrentMusic() const { return currentMusicPath; }

private:
    AudioManager();
    ~AudioManager();

    void Initialize();
    void Cleanup();

    bool initialized = false;
    bool soundEnabled = true;
    int masterVolume = 128;

    // Keep track of what sound is playing on each channel
    std::map<int, std::string> channelMap;
    // Track current music
    std::shared_ptr<Mix_Music> currentMusic;
    std::string currentMusicPath;

    // Channel finished callback
    static void ChannelFinishedCallback(int channel);
    void OnChannelFinished(int channel);

    friend void ChannelFinishedCallback(int channel);
};