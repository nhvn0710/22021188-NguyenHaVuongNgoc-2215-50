#include "game.h"

#include <cstdio>

using namespace std;

Game::Game() : gameWon(false), elapsedSeconds(0), finalTime(0), remainingFlags(0), window(nullptr), renderer(nullptr),
               font(nullptr), currentState(GameState::MAIN_MENU), board(10, 10, 10), currentDifficulty()
{
    loadHighScores();
	startButton = {{SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2, 200, BUTTON_WIDTH, BUTTON_HEIGHT}, "Start Game"};
	quitButton = {{SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2, 400, BUTTON_WIDTH, BUTTON_HEIGHT}, "Quit"};
	backButton = {{20, 20, BUTTON_WIDTH / 2, BUTTON_HEIGHT / 2}, "Back"};
	newGameButton = {{SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2, 300, BUTTON_WIDTH, BUTTON_HEIGHT}, "New Game"};
	easyButton = {{20, 150, BUTTON_WIDTH / 3, BUTTON_HEIGHT / 3}, "Easy"};
	mediumButton = {{20, 200, BUTTON_WIDTH / 3, BUTTON_HEIGHT / 3}, "Medium"};
	hardButton = {{20, 250, BUTTON_WIDTH / 3, BUTTON_HEIGHT / 3}, "Hard"};
    highScoresButton = {{SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2, 300, BUTTON_WIDTH, BUTTON_HEIGHT},"High Scores"};
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

    window = SDL_CreateWindow("Honehoover", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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

    if (window == nullptr || renderer == nullptr || font == nullptr) {
        printf("Failed to load SDL components: %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        exit(1);
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

void Game::setDifficulty(DifficultyLevel level) {
    switch (level) {
    case DifficultyLevel::EASY:
        board = Board(12, 9, 12);
        break;
    case DifficultyLevel::MEDIUM:
        board = Board(20, 15, 30);
        break;
    case DifficultyLevel::HARD:
        board = Board(30, 24, 120);
        break;
    }
    currentDifficulty = level;
}

string Game::difficultyToString(DifficultyLevel level) {
    switch (level) {
    case DifficultyLevel::EASY:
        return "Easy";
    case DifficultyLevel::MEDIUM:
        return "Medium";
    case DifficultyLevel::HARD:
        return "Hard";
    default:
        return "Unknown Difficulty";
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
                if (isMouseOverButton(easyButton, mouseX, mouseY)) {
                    setDifficulty(DifficultyLevel::EASY);
                }
                else if (isMouseOverButton(mediumButton, mouseX, mouseY)) {
                    setDifficulty(DifficultyLevel::MEDIUM);
                }
                else if (isMouseOverButton(hardButton, mouseX, mouseY)) {
                    setDifficulty(DifficultyLevel::HARD);
                }
                else if (isMouseOverButton(startButton, mouseX, mouseY)) {
                    currentState = GameState::PLAYING; // Change to PLAYING state but don't reset
                }
                else if (isMouseOverButton(highScoresButton, mouseX, mouseY) && e.button.button == SDL_BUTTON_LEFT) {
                    currentState = GameState::VIEW_HIGH_SCORES;
                }
                else if (isMouseOverButton(quitButton, mouseX, mouseY)) {
                    currentState = GameState::QUIT;
                }
                break;
            case GameState::VIEW_HIGH_SCORES: {
                    if (isMouseOverButton(backButton, mouseX, mouseY) && e.button.button == SDL_BUTTON_LEFT) {
                        currentState = GameState::MAIN_MENU;
                    }
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
                                gameWon = false;
                                board.revealAllMines();
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
                    resetGame();
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
            gameTimer.stop();
            finalTime = elapsedSeconds;
            updateHighScores();
        }

        // Check for lose condition
        if (board.isGameOver()) {
            currentState = GameState::GAME_OVER;
            gameWon = false;
            board.revealAllMines();
            gameTimer.stop();
        }

        // Update game timer
        if (!gameTimer.isStarted()) {
            gameTimer.start();
        }
        elapsedSeconds = gameTimer.getElapsedTime() / 1000;

        // Update flag count
        remainingFlags = board.getMineCount() - board.getFlagCount();
    }
}

void Game::resetGame() {
    board.reset();
    gameWon = false;
    elapsedSeconds = 0;
    finalTime = 0;
    remainingFlags = board.getMineCount();
    gameTimer.stop();
    currentState = GameState::PLAYING;
}

void Game::cleanUp() {
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}