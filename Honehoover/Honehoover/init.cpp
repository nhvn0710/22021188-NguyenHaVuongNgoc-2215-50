#include "game.h"
#include <iostream>

SDL_Texture* Game::loadTexture(const string& path) {
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == nullptr) {
        cout << "Unable to load image " << path << "! SDL_image Error: " << IMG_GetError() << endl;
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if (texture == nullptr) {
        cout << "Unable to create texture from " << path << "! SDL Error: " << SDL_GetError() << endl;
    }

    SDL_FreeSurface(loadedSurface);
    return texture;
}

void Game::loadTextures() {
    textures["antiq"] = loadTexture("../resources/image/antiq.png");
    textures["cross"] = loadTexture("../resources/image/cross.png");
    textures["frame"] = loadTexture("../resources/image/frame.png");
    textures["grass"] = loadTexture("../resources/image/grass.png");
    textures["clean"] = loadTexture("../resources/image/clean.png");
    textures["pylon"] = loadTexture("../resources/image/pylon.png");
    textures["steel"] = loadTexture("../resources/image/steel.png");
    textures["btton"] = loadTexture("../resources/image/btton.png");
    textures["bttn1"] = loadTexture("../resources/image/bttn1.png");
    textures["bttn2"] = loadTexture("../resources/image/bttn2.png");
    textures["mmnbg"] = loadTexture("../resources/image/mmnbg.png");
    textures["ldbbg"] = loadTexture("../resources/image/ldbbg.png");
}

void Game::renderTexture(const string& textureId, int x, int y, int width, int height) {
    if (textures.find(textureId) == textures.end()) {
        cout << "Texture " << textureId << " not found!" << endl;
        return;
    }

    SDL_Rect renderQuad = { x, y, width, height };

    SDL_RenderCopy(renderer, textures[textureId], nullptr, &renderQuad);
}

void Game::freeTextures() {
    for (auto& texture : textures) {
        if (texture.second != nullptr) {
            SDL_DestroyTexture(texture.second);
            texture.second = nullptr;
        }
    }
}

Mix_Music* Game::loadMixerMusic(const string& path)
{
    Mix_Music* loadedMusic = Mix_LoadMUS(path.c_str());
    if (loadedMusic == nullptr) {
        cout << "Unable to load music " << path << "! SDL_mixer Error: " << Mix_GetError() << endl;
        return nullptr;
    }

    return loadedMusic;
}

Mix_Chunk* Game::loadMixerChunk(const string& path)
{
    Mix_Chunk* loadedChunk = Mix_LoadWAV(path.c_str());
    if (loadedChunk == nullptr) {
        cout << "Unable to load chunk " << path << "! SDL_mixer Error: " << Mix_GetError() << endl;
        return nullptr;
    }

    return loadedChunk;
}

void Game::loadMixers()
{
    backgroundMusic = loadMixerMusic("../resources/mixer/song03.mp3");
    gameplayMusic = loadMixerMusic("../resources/mixer/song01.mp3");
    leaderboardMusic = loadMixerMusic("../resources/mixer/song00.mp3");
    cellRevealSound = loadMixerChunk("../resources/mixer/cellrevealsound.wav");
    flagToggleSound = loadMixerChunk("../resources/mixer/flagsound.wav");
    gameWinSound = loadMixerMusic("../resources/mixer/winsound.wav");
    gameLoseSound = loadMixerMusic("../resources/mixer/losesound.wav");
    buttonClickSound = loadMixerChunk("../resources/mixer/button.mp3");
}