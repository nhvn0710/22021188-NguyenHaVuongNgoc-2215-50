//A build is supposed to be pushed up today but it's bugging fsr Sadge

#include <SDL.h>
#include <SDL_ttf.h>
#include <cstdio>
#include <string>

using namespace std;

constexpr int SCREEN_WIDTH = 640;
constexpr int SCREEN_HEIGHT = 480;
constexpr int BUTTON_WIDTH = 200;
constexpr int BUTTON_HEIGHT = 50;

enum GameState {
    MAIN_MENU,
    GAME_SCREEN,
    QUIT
};

struct Button {
    SDL_Rect rect;
    string text;
};

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
TTF_Font* font = nullptr;
GameState currentState = MAIN_MENU;

Button startButton = { {SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2, 200, BUTTON_WIDTH, BUTTON_HEIGHT}, "Start Game" };
Button quitButton = { {SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2, 300, BUTTON_WIDTH, BUTTON_HEIGHT}, "Quit" };
Button backButton = { {20, 20, BUTTON_WIDTH / 2, BUTTON_HEIGHT / 2}, "Back" };

void renderText(const string& text, int x, int y, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    int width, height;
    SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
    SDL_Rect dstRect = { x, y, width, height };

    SDL_RenderCopy(renderer, texture, nullptr, &dstRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void renderButton(const Button& button) {
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderFillRect(renderer, &button.rect);

    SDL_Color textColor = { 0, 0, 0, 255 };
    renderText(button.text, button.rect.x + 10, button.rect.y + 10, textColor);
}

void renderMainMenu() {
    SDL_SetRenderDrawColor(renderer, 0xAA, 0xFF, 0xDD, 255);
    SDL_RenderClear(renderer);

    renderButton(startButton);
    renderButton(quitButton);

    SDL_Color titleColor = { 0, 0, 0, 255 };
    renderText("Honehoover", SCREEN_WIDTH / 2 - 100, 100, titleColor);
}

void renderGameScreen() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    renderButton(backButton);

    SDL_Color textColor = { 0, 0, 0, 255 };
    renderText("Honehoover Game (To Be Added)", SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2, textColor);
}

bool isMouseOverButton(const Button& button, int mouseX, int mouseY) {
    return (mouseX >= button.rect.x && mouseX <= button.rect.x + button.rect.w &&
        mouseY >= button.rect.y && mouseY <= button.rect.y + button.rect.h);
}

int main(int argc, char* args[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() < 0) {
        printf("TTF could not initialize! TTF_Error: %s\n", TTF_GetError());
        return 1;
    }

    window = SDL_CreateWindow("Honehoover (Minesweeper)", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    font = TTF_OpenFont("../resources/font/Restore-W00-Heavy.ttf", 24);
    if (font == nullptr) {
        printf("Failed to load font! TTF_Error: %s\n", TTF_GetError());
        return 1;
    }

    SDL_Event e;
    bool quit = false;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                if (currentState == MAIN_MENU) {
                    if (isMouseOverButton(startButton, mouseX, mouseY)) {
                        currentState = GAME_SCREEN;
                    }
                    else if (isMouseOverButton(quitButton, mouseX, mouseY)) {
                        quit = true;
                    }
                }
                else if (currentState == GAME_SCREEN) {
                    if (isMouseOverButton(backButton, mouseX, mouseY)) {
                        currentState = MAIN_MENU;
                    }
                }
            }
        }

        if (currentState == MAIN_MENU) {
            renderMainMenu();
        }
        else if (currentState == GAME_SCREEN) {
            renderGameScreen();
        }

        SDL_RenderPresent(renderer);
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}