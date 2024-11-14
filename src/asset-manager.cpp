#include "asset-manager.h"

AssetManager& AssetManager::Instance() {
    static AssetManager instance;
    return instance;
}

std::shared_ptr<SDL_Texture> AssetManager::LoadTexture(const std::string& path, SDL_Renderer* renderer) {
    auto it = textures.find(path);
    if (it != textures.end()) {
        return it->second;
    }
    
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        return nullptr;
    }
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    
    if (!texture) {
        return nullptr;
    }
    
    auto shared_texture = std::shared_ptr<SDL_Texture>(texture, SDL_DestroyTexture);
    textures[path] = shared_texture;
    return shared_texture;
}

std::shared_ptr<Mix_Chunk> AssetManager::LoadSound(const std::string& path) {
    auto it = sounds.find(path);
    if (it != sounds.end()) {
        return it->second;
    }
    
    Mix_Chunk* chunk = Mix_LoadWAV(path.c_str());
    if (!chunk) {
        return nullptr;
    }
    
    auto shared_chunk = std::shared_ptr<Mix_Chunk>(chunk, Mix_FreeChunk);
    sounds[path] = shared_chunk;
    return shared_chunk;
}

std::shared_ptr<Mix_Music> AssetManager::LoadMusic(const std::string& path) {
    auto it = music.find(path);
    if (it != music.end()) {
        return it->second;
    }
    
    Mix_Music* mus = Mix_LoadMUS(path.c_str());
    if (!mus) {
        return nullptr;
    }
    
    auto shared_music = std::shared_ptr<Mix_Music>(mus, Mix_FreeMusic);
    music[path] = shared_music;
    return shared_music;
}

void AssetManager::ClearAssets() {
    textures.clear();
    sounds.clear();
    music.clear();
}
