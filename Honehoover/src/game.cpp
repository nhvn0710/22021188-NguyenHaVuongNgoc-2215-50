#include "game.h"
#include <cstdio>

using namespace std;

Game::Game() : gameWon(false), elapsedSeconds(0), remainingFlags(0), window(nullptr), renderer(nullptr), font(nullptr), currentState(GameState::MAIN_MENU), board(10, 10, 10) {
    startButton = { {SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2, 200, BUTTON_WIDTH, BUTTON_HEIGHT}, "Start Game" };
    quitButton = { {SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2, 300, BUTTON_WIDTH, BUTTON_HEIGHT}, "Quit" };
    backButton = { {20, 20, BUTTON_WIDTH / 2, BUTTON_HEIGHT / 2}, "Back" };
    newGameButton = { {SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2, 300, BUTTON_WIDTH, BUTTON_HEIGHT}, "New Game" };
}

Game::~Game() {
    cleanUp();
}

bool Game::initialize() {
    if (!initSDL()) {
        return false;
    }
    return true;
}

bool Game::initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    if (TTF_Init() < 0) {
        printf("TTF could not initialize! TTF_Error: %s\n", TTF_GetError());
        return false;
    }

    window = SDL_CreateWindow("Minesweeper", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    font = TTF_OpenFont("../resources/font/Restore-W00-Heavy.ttf", 24);
    if (font == nullptr) {
        printf("Failed to load font! TTF_Error: %s\n", TTF_GetError());
        return false;
    }

    return true;
}

void Game::run() {
    while (currentState != GameState::QUIT) {
        handleEvents();
        update();
        render();
    }
}

void Game::handleEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            currentState = GameState::QUIT;
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN) {
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);

            switch (currentState) {
            case GameState::MAIN_MENU:
                if (isMouseOverButton(startButton, mouseX, mouseY)) {
                    currentState = GameState::PLAYING;
                    board.reset();
                }
                else if (isMouseOverButton(quitButton, mouseX, mouseY)) {
                    currentState = GameState::QUIT;
                }
                break;
            case GameState::PLAYING:
                if (isMouseOverButton(backButton, mouseX, mouseY)) {
                    currentState = GameState::MAIN_MENU;
                }
                else {
                    int cellX = mouseX / (SCREEN_WIDTH / board.getWidth());
                    int cellY = mouseY / (SCREEN_HEIGHT / board.getHeight());
                    if (e.button.button == SDL_BUTTON_LEFT) {
                        if (board.revealCell(cellX, cellY)) {
                            if (board.isGameOver()) {
                                currentState = GameState::GAME_OVER;
                            }
                        }
                    }
                    else if (e.button.button == SDL_BUTTON_RIGHT) {
                        board.toggleFlag(cellX, cellY);
                    }
                }
                break;
            case GameState::GAME_OVER:
                if (isMouseOverButton(newGameButton, mouseX, mouseY)) {
                    currentState = GameState::PLAYING;
                    board.reset();
                }
                else if (isMouseOverButton(backButton, mouseX, mouseY)) {
                    currentState = GameState::MAIN_MENU;
                }
                break;
            }
        }
    }
}

void Game::update() {
    if (currentState == GameState::PLAYING) {
        // Check for win condition
        if (board.isGameWon()) {
            currentState = GameState::GAME_OVER;
            gameWon = true;
        }

        // Check for lose condition
        if (board.isGameOver()) {
            currentState = GameState::GAME_OVER;
            gameWon = false;
            board.revealAllMines();
        }

        // Update game timer
        if (!gameTimer.isStarted()) {
            gameTimer.start();
        }
        elapsedSeconds = gameTimer.getElapsedTime() / 1000;  // Convert to seconds

        // Update flag count
        remainingFlags = board.getMineCount() - board.getFlagCount();
    }
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    switch (currentState) {
    case GameState::MAIN_MENU:
        renderMainMenu();
        break;
    case GameState::PLAYING:
        renderGameScreen();
        break;
    case GameState::GAME_OVER:
        renderGameOverScreen();
        break;
    }

    SDL_RenderPresent(renderer);
}

void Game::renderText(const string& text, int x, int y, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    int width, height;
    SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
    SDL_Rect dstRect = { x, y, width, height };

    SDL_RenderCopy(renderer, texture, nullptr, &dstRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void Game::renderButton(const Button& button) {
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderFillRect(renderer, &button.rect);

    SDL_Color textColor = { 0, 0, 0, 255 };
    renderText(button.text, button.rect.x + 10, button.rect.y + 10, textColor);
}

bool Game::isMouseOverButton(const Button& button, int mouseX, int mouseY) {
    return (mouseX >= button.rect.x && mouseX <= button.rect.x + button.rect.w &&
        mouseY >= button.rect.y && mouseY <= button.rect.y + button.rect.h);
}

void Game::renderMainMenu() {
    SDL_SetRenderDrawColor(renderer, 0xAA, 0xFF, 0xDD, 255);
    SDL_RenderClear(renderer);

    renderButton(startButton);
    renderButton(quitButton);

    SDL_Color titleColor = { 0, 0, 0, 255 };
    renderText("Minesweeper", SCREEN_WIDTH / 2 - 100, 100, titleColor);
}

void Game::renderGameScreen() {
    board.render(renderer, font, SCREEN_WIDTH, SCREEN_HEIGHT);
    renderButton(backButton);
}

void Game::renderGameOverScreen() {
    SDL_SetRenderDrawColor(renderer, 255, 200, 200, 255);
    SDL_RenderClear(renderer);

    SDL_Color textColor = { 0, 0, 0, 255 };
    renderText("Game Over!", SCREEN_WIDTH / 2 - 100, 100, textColor);
    renderButton(newGameButton);
    renderButton(backButton);
}

void Game::cleanUp() {
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}