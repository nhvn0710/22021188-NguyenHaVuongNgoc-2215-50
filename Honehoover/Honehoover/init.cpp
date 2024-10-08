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

void Game::loadFonts() {
    if (altTexture == false) {
        font = TTF_OpenFont("../resources/font/YomiyasuSansSerif-Rounded.otf", 34);
    }
    else {
        font = TTF_OpenFont("../resources/font/SmashSans-Metaverse.otf", 34);
    }
}

void Game::loadTextures() {
    if (altTexture == false) {
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
        textures["govbg"] = loadTexture("../resources/image/govbg.png");

        textures["music_on"] = loadTexture("../resources/image/music_on.png");
        textures["music_off"] = loadTexture("../resources/image/music_off.png");
        textures["sound_on"] = loadTexture("../resources/image/sound_on.png");
        textures["sound_off"] = loadTexture("../resources/image/sound_off.png");
        textures["extra_life_on"] = loadTexture("../resources/image/extralive_on.png");
        textures["extra_life_off"] = loadTexture("../resources/image/extralive_off.png");
        textures["textures_on"] = loadTexture("../resources/image/texture_on.png");
        textures["textures_off"] = loadTexture("../resources/image/texture_off.png");
    } else
    {
        textures["antiq"] = loadTexture("../resources/image/antiq2.png");
        textures["cross"] = loadTexture("../resources/image/cross2.png");
        textures["frame"] = loadTexture("../resources/image/frame2.png");
        textures["grass"] = loadTexture("../resources/image/grass2.png");
        textures["clean"] = loadTexture("../resources/image/clean2.png");
        textures["pylon"] = loadTexture("../resources/image/pylon2.png");
        textures["steel"] = loadTexture("../resources/image/steel2.png");
        textures["btton"] = loadTexture("../resources/image/btton2.png");
        textures["bttn1"] = loadTexture("../resources/image/bttn12.png");
        textures["bttn2"] = loadTexture("../resources/image/bttn22.png");
        textures["mmnbg"] = loadTexture("../resources/image/mmnbg2.png");
        textures["ldbbg"] = loadTexture("../resources/image/ldbbg2.png");
        textures["govbg"] = loadTexture("../resources/image/govbg2.png");

        textures["music_on"] = loadTexture("../resources/image/music_on2.png");
        textures["music_off"] = loadTexture("../resources/image/music_off2.png");
        textures["sound_on"] = loadTexture("../resources/image/sound_on2.png");
        textures["sound_off"] = loadTexture("../resources/image/sound_off2.png");
        textures["extra_life_on"] = loadTexture("../resources/image/extralive_on2.png");
        textures["extra_life_off"] = loadTexture("../resources/image/extralive_off2.png");
        textures["textures_on"] = loadTexture("../resources/image/texture_on2.png");
        textures["textures_off"] = loadTexture("../resources/image/texture_off2.png");
    }
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