#include "game.h"

#include <algorithm>
#include <cstdio>
#include <fstream>
#include <iostream>

using namespace std;

Game::Game() : gameWon(false), elapsedSeconds(0), finalTime(0), remainingFlags(0), window(nullptr), renderer(nullptr),
               font(nullptr), currentState(GameState::MAIN_MENU), board(10, 10, 10), currentDifficulty()
{
    loadHighScores();
	startButton = {{SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2, 200, BUTTON_WIDTH, BUTTON_HEIGHT}, "Start Game"};
	quitButton = {{SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2, 300, BUTTON_WIDTH, BUTTON_HEIGHT}, "Quit"};
	backButton = {{20, 20, BUTTON_WIDTH / 2, BUTTON_HEIGHT / 2}, "Back"};
	newGameButton = {{SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2, 300, BUTTON_WIDTH, BUTTON_HEIGHT}, "New Game"};
	easyButton = {{20, 150, BUTTON_WIDTH / 3, BUTTON_HEIGHT / 3}, "Easy"};
	mediumButton = {{20, 200, BUTTON_WIDTH / 3, BUTTON_HEIGHT / 3}, "Medium"};
	hardButton = {{20, 250, BUTTON_WIDTH / 3, BUTTON_HEIGHT / 3}, "Hard"};
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
        board = Board(8, 8, 5);
        break;
    case DifficultyLevel::MEDIUM:
        board = Board(16, 16, 15);
        break;
    case DifficultyLevel::HARD:
        board = Board(24, 24, 40);
        break;
    }
    currentDifficulty = level;
    resetGame();
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
                if (isMouseOverButton(startButton, mouseX, mouseY)) {
                    resetGame();
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
        if (gameWon) {
            renderWinScreen();
        }
        else {
            renderGameOverScreen();
        }
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

void Game::renderCenteredText(const string& text, int y, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    int textWidth = surface->w;
    int textHeight = surface->h;
    SDL_Rect textRect = { (SCREEN_WIDTH - textWidth) / 2, y, textWidth, textHeight };

    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, texture, nullptr, &textRect);
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
    renderButton(easyButton);
    renderButton(mediumButton);
    renderButton(hardButton);

    SDL_Color titleColor = { 0, 0, 0, 255 };
    renderText("Honehoover", SCREEN_WIDTH / 2 - 100, 100, titleColor);

    displayHighScores();
}

void Game::renderGameScreen() {
    board.render(renderer, font, SCREEN_WIDTH, SCREEN_HEIGHT);
    renderButton(backButton);

    SDL_Color textColor = { 0, 0, 0, 255 };
    string timeText = "Time: " + to_string(elapsedSeconds) + " s";
    renderText(timeText, 10, SCREEN_HEIGHT - 30, textColor);

    string flagText = "Flags: " + to_string(remainingFlags);
    renderText(flagText, SCREEN_WIDTH - 120, SCREEN_HEIGHT - 30, textColor);
}

void Game::renderGameOverScreen() {
    SDL_SetRenderDrawColor(renderer, 255, 200, 200, 255);
    SDL_RenderClear(renderer);

    SDL_Color textColor = { 0, 0, 0, 255 };
    renderText("Game Over!", SCREEN_WIDTH / 2 - 100, 100, textColor);
    renderText("You Lost!", SCREEN_WIDTH / 2 - 80, 150, textColor);
    renderButton(newGameButton);
    renderButton(backButton);
}

void Game::renderWinScreen() {
    SDL_SetRenderDrawColor(renderer, 200, 255, 200, 255);
    SDL_RenderClear(renderer);

    SDL_Color textColor = { 0, 0, 0, 255 };

    string timeText = "Time: " + to_string(finalTime) + " seconds";
    renderText(timeText, SCREEN_WIDTH / 2 - 100, 200, textColor);

    renderButton(newGameButton);
    renderButton(backButton);
    string victoryMessage = "You cleared all mines! Well done!";
    renderCenteredText(victoryMessage, SCREEN_HEIGHT / 2 + 50, textColor);
    displayHighScores();
}

void Game::loadHighScores() {
    ifstream inFile("highscores.txt");
    highScores.clear();

    int time;
    string difficulty;
    while (inFile >> time >> difficulty) {
        DifficultyLevel difficultyLevel = DifficultyLevel::EASY;
        if (difficulty == "Easy") difficultyLevel = DifficultyLevel::EASY;
        else if (difficulty == "Medium") difficultyLevel = DifficultyLevel::MEDIUM;
        else if (difficulty == "Hard") difficultyLevel = DifficultyLevel::HARD;

        highScores.push_back({ time, difficultyLevel });
    }

    sort(highScores.begin(), highScores.end(), compareHighScores);
}

void Game::updateHighScores() {
    if (gameWon) {
        HighScore newScore{};
        newScore.time = finalTime;
        newScore.difficulty = currentDifficulty;

        highScores.push_back(newScore);
        sort(highScores.begin(), highScores.end(), compareHighScores);
        cout << "added" << endl;

        // Trim list to top 5
        if (highScores.size() > 5) {
            highScores.resize(5);
        }

        saveHighScores();
    }
}

void Game::saveHighScores() const {
    ofstream outFile("highscores.txt");
    if (outFile) {
        for (const auto& score : highScores) {
            string difficultyName;
            switch (score.difficulty) {
            case DifficultyLevel::EASY:
                difficultyName = "Easy";
                break;
            case DifficultyLevel::MEDIUM:
                difficultyName = "Medium";
                break;
            case DifficultyLevel::HARD:
                difficultyName = "Hard";
                break;
            }
            outFile << to_string(score.time) << " " << difficultyName << "\n";
        }
    }
}


bool Game::compareHighScores(const HighScore& a, const HighScore& b) {
    int weightA = a.difficulty == DifficultyLevel::HARD ? 3 : a.difficulty == DifficultyLevel::MEDIUM ? 2 : 1;
    int weightB = b.difficulty == DifficultyLevel::HARD ? 3 : b.difficulty == DifficultyLevel::MEDIUM ? 2 : 1;

    return a.time * weightA < b.time * weightB;
}

void Game::displayHighScores() {
    SDL_Color color = { 255, 255, 0, 255 }; 
    int yPosition = 100; 
    renderCenteredText("High Scores:", yPosition, color);
    yPosition += 30; 

    for (const auto& score : highScores) {
        string difficultyName = (score.difficulty == DifficultyLevel::EASY ? "Easy" :
            score.difficulty == DifficultyLevel::MEDIUM ? "Medium" :
            score.difficulty == DifficultyLevel::HARD ? "Hard" : "Unknown");
        string text = difficultyName + " - " + to_string(score.time) + "s";
        renderCenteredText(text, yPosition, color);
        yPosition += 30;
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