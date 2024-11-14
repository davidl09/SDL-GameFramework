#pragma once
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <unordered_map>
#include <string>
#include <memory>

class AssetManager {
public:
    static AssetManager& Instance();
    
    std::shared_ptr<SDL_Texture> LoadTexture(const std::string& path, SDL_Renderer* renderer);
    std::shared_ptr<Mix_Chunk> LoadSound(const std::string& path);
    std::shared_ptr<Mix_Music> LoadMusic(const std::string& path);
    
    void ClearAssets();

private:
    AssetManager() = default;
    ~AssetManager() = default;
    
    std::unordered_map<std::string, std::shared_ptr<SDL_Texture>> textures;
    std::unordered_map<std::string, std::shared_ptr<Mix_Chunk>> sounds;
    std::unordered_map<std::string, std::shared_ptr<Mix_Music>> music;
};
