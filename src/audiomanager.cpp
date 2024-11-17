//
// Created by David Laeer on 2024-11-16.
//

#include "audiomanager.h"
#include "asset-manager.h"

#include <stdexcept>

void AudioManager::ChannelFinishedCallback(int channel) {
    Instance().OnChannelFinished(channel);
}

AudioManager& AudioManager::Instance() {
    static AudioManager instance;
    return instance;
}

AudioManager::AudioManager() {
    Initialize();
}

AudioManager::~AudioManager() {
    Cleanup();
}

void AudioManager::Initialize() {
    if (initialized) return;

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        throw std::runtime_error("SDL_mixer initialization failed: " + std::string(Mix_GetError()));
    }

    Mix_AllocateChannels(16);
    Mix_ChannelFinished(ChannelFinishedCallback);
    initialized = true;
}

void AudioManager::Cleanup() {
    if (!initialized) return;

    StopAll();
    channelMap.clear();
    currentMusic = nullptr;
    currentMusicPath.clear();

    Mix_CloseAudio();
    initialized = false;
}

int AudioManager::PlaySound(const std::string& path, const PlayConfig& config) {
    if (!soundEnabled || !initialized) return -1;

    try {
        auto sound = AssetManager::Instance().LoadSound(path);
        if (!sound) return -1;

        // Apply volume with master volume scaling
        int scaledVolume = static_cast<int>(config.volume * (masterVolume / 128.0f));
        Mix_VolumeChunk(sound.get(), scaledVolume);

        // Play the sound
        int channel;
        if (config.fadeInMs > 0) {
            channel = Mix_FadeInChannel(config.channel, sound.get(), config.loops, config.fadeInMs);
        } else {
            channel = Mix_PlayChannel(config.channel, sound.get(), config.loops);
        }

        if (channel == -1) {
            SDL_LogError(SDL_LOG_CATEGORY_AUDIO, "Failed to play sound: %s", Mix_GetError());
            return -1;
        }

        // Track the sound path for this channel
        channelMap[channel] = path;
        return channel;

    } catch (const std::exception& e) {
        SDL_LogError(SDL_LOG_CATEGORY_AUDIO, "Failed to play sound: %s", e.what());
        return -1;
    }
}

int AudioManager::PlayLoopedSound(const std::string& path, int volume) {
    PlayConfig config;
    config.loops = -1;  // Infinite loop
    config.volume = volume;
    return PlaySound(path, config);
}

void AudioManager::StopSound(int channel, int fadeOutMs) {
    if (!initialized) return;

    if (fadeOutMs > 0) {
        Mix_FadeOutChannel(channel, fadeOutMs);
    } else {
        Mix_HaltChannel(channel);
    }
}

void AudioManager::StopSound(const std::string& path, int fadeOutMs) {
    for (const auto& [channel, soundPath] : channelMap) {
        if (soundPath == path) {
            StopSound(channel, fadeOutMs);
        }
    }
}

void AudioManager::SetSoundVolume(int channel, int volume) {
    if (!initialized) return;
    Mix_Volume(channel, static_cast<int>(volume * (masterVolume / 128.0f)));
}

bool AudioManager::IsSoundPlaying(int channel) const {
    return Mix_Playing(channel) == 1;
}

void AudioManager::PauseSound(int channel) {
    Mix_Pause(channel);
}

void AudioManager::ResumeSound(int channel) {
    Mix_Resume(channel);
}

void AudioManager::PlayMusic(const std::string& path, const PlayConfig& config) {
    if (!soundEnabled || !initialized) return;

    try {
        currentMusic = AssetManager::Instance().LoadMusic(path);
        if (!currentMusic) return;

        Mix_VolumeMusic(static_cast<int>(config.volume * (masterVolume / 128.0f)));

        if (config.fadeInMs > 0) {
            if (Mix_FadeInMusic(currentMusic.get(), config.loops, config.fadeInMs) == -1) {
                throw std::runtime_error(Mix_GetError());
            }
        } else {
            if (Mix_PlayMusic(currentMusic.get(), config.loops) == -1) {
                throw std::runtime_error(Mix_GetError());
            }
        }

        currentMusicPath = path;

    } catch (const std::exception& e) {
        SDL_LogError(SDL_LOG_CATEGORY_AUDIO, "Failed to play music: %s", e.what());
    }
}

void AudioManager::StopMusic(int fadeOutMs) {
    if (!initialized) return;

    if (fadeOutMs > 0 && Mix_PlayingMusic()) {
        Mix_FadeOutMusic(fadeOutMs);
    } else {
        Mix_HaltMusic();
    }
    currentMusic = nullptr;
    currentMusicPath.clear();
}

void AudioManager::PauseMusic() {
    if (initialized && Mix_PlayingMusic()) {
        Mix_PauseMusic();
    }
}

void AudioManager::ResumeMusic() {
    if (initialized && Mix_PausedMusic()) {
        Mix_ResumeMusic();
    }
}

void AudioManager::SetMusicVolume(int volume) {
    if (initialized) {
        Mix_VolumeMusic(static_cast<int>(volume * (masterVolume / 128.0f)));
    }
}

bool AudioManager::IsMusicPlaying() const {
    return Mix_PlayingMusic() == 1;
}

bool AudioManager::IsMusicPaused() const {
    return Mix_PausedMusic() == 1;
}

int AudioManager::FindChannel(const std::string& path) const {
    for (const auto& [channel, soundPath] : channelMap) {
        if (soundPath == path) {
            return channel;
        }
    }
    return -1;
}

void AudioManager::StopAll(int fadeOutMs) {
    if (!initialized) return;

    // Stop all channels
    if (fadeOutMs > 0) {
        Mix_FadeOutChannel(-1, fadeOutMs);
    } else {
        Mix_HaltChannel(-1);
    }

    // Stop music
    StopMusic(fadeOutMs);

    channelMap.clear();
}

void AudioManager::PauseAll() {
    Mix_Pause(-1);  // Pause all channels
    PauseMusic();
}

void AudioManager::ResumeAll() {
    Mix_Resume(-1);  // Resume all channels
    ResumeMusic();
}

void AudioManager::SetMasterVolume(int volume) {
    masterVolume = std::clamp(volume, 0, 128);

    // Update all playing sounds
    for (const auto& [channel, _] : channelMap) {
        if (Mix_Playing(channel)) {
            Mix_Volume(channel, Mix_Volume(channel, -1) * masterVolume / 128);
        }
    }

    // Update music
    if (Mix_PlayingMusic()) {
        SetMusicVolume(Mix_VolumeMusic(-1));
    }
}

void AudioManager::EnableSound(bool enabled) {
    soundEnabled = enabled;
    if (!enabled) {
        StopAll();
    }
}

int AudioManager::ReserveChannels(int num) {
    return Mix_ReserveChannels(num);
}

void AudioManager::UnreserveChannels(int num) {
    Mix_ReserveChannels(Mix_ReserveChannels(-1) - num);
}

int AudioManager::GetMusicVolume() const {
    return Mix_VolumeMusic(-1);
}

void AudioManager::OnChannelFinished(int channel) {
    channelMap.erase(channel);
}
