#include "game.h"

#include <cstdio>
#include <iostream>

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

const Game::Difficulty Game::difficulties[] = {
    { 12, 9, 4 },
    { 20, 15, 20 }, 
    { 30, 24, 184 }
};

void Game::setDifficulty(DifficultyLevel level) {
    switch (level) {
    case DifficultyLevel::EASY:
        board = Board(12, 9, 4);
        break;
    case DifficultyLevel::MEDIUM:
        board = Board(20, 15, 20);
        break;
    case DifficultyLevel::HARD:
        board = Board(30, 24, 184);
        break;
    case DifficultyLevel::CUSTOM:
            board = Board(customDifficulty.width, customDifficulty.height, customDifficulty.mineCount);
            if (customDifficulty == difficulties[0]) { //0
                currentDifficulty = DifficultyLevel::EASY;
            }
            else if (customDifficulty == difficulties[1]) { //32
                currentDifficulty = DifficultyLevel::MEDIUM;
            }
            else if (customDifficulty == difficulties[2]) { //72
                currentDifficulty = DifficultyLevel::HARD;
            }
            else {
                currentDifficulty = DifficultyLevel::CUSTOM;
            }
            break;
    }
    currentDifficulty = level;
}

void Game::setCustomDifficulty(int sliderVal)
{
    int width = 12 + sliderVal * 5 / 20;
    int height = 9 + sliderVal * 5 / 24;
    int mineCount = 4 + sliderVal * (0.5 + (sliderVal * sliderVal) / 2000);
    customDifficulty = { width, height, mineCount, true };
}

string Game::difficultyToString(DifficultyLevel level) {
    switch (level) {
    case DifficultyLevel::EASY:
        return "Easy";
    case DifficultyLevel::MEDIUM:
        return "Medium";
    case DifficultyLevel::HARD:
        return "Hard";
    case DifficultyLevel::CUSTOM:
        return "Custom";
    default:
        return "Unknown Difficulty";
    }
}

int Game::getDifficultySliderValue(DifficultyLevel level) {
    switch (level) {
    case DifficultyLevel::EASY:
        return 0; 
    case DifficultyLevel::MEDIUM:
        return 32;
    case DifficultyLevel::HARD:
        return 72; 
    default:
        return -1;
    }
}

Game::DifficultyLevel Game::getCurrentDifficulty(int sliderVal) {
    if (abs(sliderVal - getDifficultySliderValue(DifficultyLevel::EASY)) < 5) {
        return DifficultyLevel::EASY;
    }
    else if (abs(sliderVal - getDifficultySliderValue(DifficultyLevel::MEDIUM)) < 5) {
        return DifficultyLevel::MEDIUM;
    }
    else if (abs(sliderVal - getDifficultySliderValue(DifficultyLevel::HARD)) < 5){
        return DifficultyLevel::HARD;
    }
    return DifficultyLevel::CUSTOM;
}

template<typename T>
const T& clamp(const T& val, const T& lower, const T& upper) {
    if (val < lower) return lower;
    else if (val > upper) return upper;
    else return val;
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
                else if (mouseY >= 540 && mouseY <= 570) {
                    sliderValue = clamp((mouseX - 20) * 100 / (SCREEN_WIDTH - 60), 0, 100);
                    DifficultyLevel now = getCurrentDifficulty(sliderValue);
                    
                    // Set to closest default difficulty if within snap range
                    if (now == DifficultyLevel::CUSTOM) {
                        cout << "Custom difficulty set: " << sliderValue << "%" << endl;
                        sliderValue = clamp((mouseX - 20) * 100 / (SCREEN_WIDTH - 60), 0, 100);
                        setCustomDifficulty(sliderValue);
                    }
                    else {
                        cout << "Custom difficulty set to default: " << difficultyToString(getCurrentDifficulty(sliderValue)) << endl;
                        sliderValue = getDifficultySliderValue(now);
                    }
                	setDifficulty(now);
                }
                else if (isMouseOverButton(startButton, mouseX, mouseY)) {
                    currentState = GameState::PLAYING;
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