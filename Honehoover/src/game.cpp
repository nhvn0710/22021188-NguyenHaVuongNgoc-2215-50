#include "game.h"
#include <iostream>

using namespace std;

Game::Game() : window(nullptr), renderer(nullptr), font(nullptr), currentState(GameState::MAIN_MENU), board(12, 9, 4),
               currentDifficulty(), gameWon(false),
               elapsedSeconds(0), finalTime(0), remainingFlags(0), isNewHighScore(false)
{
    loadHighScores();
    loadTutorialSteps();
	startButton = {{SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2, 200, BUTTON_WIDTH, BUTTON_HEIGHT}, "Start Game"};
	highScoresButton = {{SCREEN_WIDTH / 2 + BUTTON_WIDTH / 16, 300, BUTTON_WIDTH, BUTTON_HEIGHT},"High Scores"};
	quitButton = {{SCREEN_WIDTH / 2 + BUTTON_WIDTH / 16, 400, BUTTON_WIDTH, BUTTON_HEIGHT}, "      Quit   "};
    tutorialButton = { {SCREEN_WIDTH / 2 - BUTTON_WIDTH / 16 - BUTTON_WIDTH, 300, BUTTON_WIDTH, BUTTON_HEIGHT},"   Tutorial" };
    settingButton = { {SCREEN_WIDTH / 2 - BUTTON_WIDTH / 16 - BUTTON_WIDTH, 400, BUTTON_WIDTH, BUTTON_HEIGHT}, "    Setting" };
    
	newGameButton = {{SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2, 300, BUTTON_WIDTH, BUTTON_HEIGHT}, "New Game"};
	backButton = { {20, SCREEN_HEIGHT + 10, BUTTON_WIDTH / 2, BUTTON_HEIGHT / 2} };

	easyButton = {{0, 600, BUTTON_WIDTH/3, BUTTON_HEIGHT}, "Easy"};
	mediumButton = {{-18 + 9*getDifficultySliderValue(DifficultyLevel::MEDIUM), 600, BUTTON_WIDTH/3, BUTTON_HEIGHT}, "Medium"};
    hardButton = { {-18 + 9*getDifficultySliderValue(DifficultyLevel::HARD), 600, BUTTON_WIDTH/3, BUTTON_HEIGHT}, "Hard" };
	veryhardButton = {{-18 + 9*getDifficultySliderValue(DifficultyLevel::VERYHARD), 600, BUTTON_WIDTH/3, BUTTON_HEIGHT}, "Very Hard"};
}

Game::~Game() {
    cleanUp();
    freeTextures();
}

bool Game::initialize() {
    if (!initSDL()) {
        return false;
    }
    return true;
}

bool Game::initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << "\n";
        return false;
    }
    if (TTF_Init() < 0) {
        cout << "TTF could not initialize! TTF_Error: " << TTF_GetError() << "\n";
        return false;
    }
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << "\n";
        return false;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << "\n";
        return false;
    }

    window = SDL_CreateWindow("Honehoover", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, TRUE_SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        cout << "Window could not be created! SDL_Error: " << SDL_GetError() << "\n";
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << "\n";
        return false;
    }

    font = TTF_OpenFont("../resources/font/SmashSans-Metaverse.otf", 34);
    if (font == nullptr) {
        cout << "Failed to load font! TTF_Error: " << TTF_GetError() << "\n";
        return false;
    }

    if (window == nullptr || renderer == nullptr || font == nullptr) {
        cout << "Failed to load SDL components: " << SDL_GetError() << "\n";
        TTF_Quit();
        SDL_Quit();
        exit(1);
    }

    loadTextures();
    loadMixers();

    return true;
}

void Game::run() {
    while (currentState != GameState::QUIT) {
        handleEvents();
        update();
        render();
    }
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
            Mix_PlayChannel(-1, buttonClickSound, 0); 

            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);

            switch (currentState) {
            case GameState::MAIN_MENU:
                if (isMouseOverButton(easyButton, mouseX, mouseY)) {
                    setDifficulty(DifficultyLevel::EASY);
                    sliderValue = getDifficultySliderValue(DifficultyLevel::EASY);
                }
                else if (isMouseOverButton(mediumButton, mouseX, mouseY)) {
                    setDifficulty(DifficultyLevel::MEDIUM);
                    sliderValue = getDifficultySliderValue(DifficultyLevel::MEDIUM);
                }
                else if (isMouseOverButton(hardButton, mouseX, mouseY)) {
                    setDifficulty(DifficultyLevel::HARD);
                    sliderValue = getDifficultySliderValue(DifficultyLevel::HARD);
                }
                else if (isMouseOverButton(veryhardButton, mouseX, mouseY)) {
                    setDifficulty(DifficultyLevel::VERYHARD);
                    sliderValue = getDifficultySliderValue(DifficultyLevel::VERYHARD);
                }
                else if (mouseY >= SLIDER_YPOSITION && mouseY <= SLIDER_YPOSITION+30) {
                    sliderValue = clamp((mouseX - 20) * 100 / (SCREEN_WIDTH - 60), 0, 100);
                    DifficultyLevel now = getCurrentDifficulty(sliderValue);
                    
                    // Set to closest default difficulty
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
                    Mix_HaltMusic();
                    currentState = GameState::PLAYING;
                }
                else if (isMouseOverButton(highScoresButton, mouseX, mouseY) && e.button.button == SDL_BUTTON_LEFT) {
                    Mix_HaltMusic();
                	currentState = GameState::VIEW_HIGH_SCORES;
                }
                else if (isMouseOverButton(tutorialButton, mouseX, mouseY) && e.button.button == SDL_BUTTON_LEFT) {
                    Mix_HaltMusic();
                    currentTutorialStep = 0;
                    currentState = GameState::TUTORIAL;
                }
                else if (isMouseOverButton(quitButton, mouseX, mouseY)) {
                    Mix_HaltMusic();
                	currentState = GameState::QUIT;
                }
                break;
            case GameState::VIEW_HIGH_SCORES: {
                    if (isMouseOverButton(backButton, mouseX, mouseY) && e.button.button == SDL_BUTTON_LEFT) {
                        Mix_HaltMusic();
                    	currentState = GameState::MAIN_MENU;
                    }
                }
                break;
            case GameState::PLAYING:
                if (isMouseOverButton(backButton, mouseX, mouseY)) {
                    Mix_HaltMusic();
                    resetGame();
                	currentState = GameState::MAIN_MENU;
                }
                else {
                    int cellX = mouseX / (SCREEN_WIDTH / board.getWidth());
                    int cellY = mouseY / (SCREEN_HEIGHT / board.getHeight());
                    if (e.button.button == SDL_BUTTON_LEFT) {
                        if (board.revealCell(cellX, cellY)) {
                            Mix_PlayChannel(-1, cellRevealSound, 0);
                        }
                    }
                    else if (e.button.button == SDL_BUTTON_RIGHT) {
                    	Mix_PlayChannel(-1, flagToggleSound, 0);
                        board.toggleFlag(cellX, cellY);
                        bool shouldAutoReveal = board.autoRevealAdjacentCells(cellX, cellY);
                    }
                }
                break;
            case GameState::GAME_OVER:
                if (isMouseOverButton(newGameButton, mouseX, mouseY)) {
                    Mix_HaltMusic();
                    resetGame();
                }
                else if (isMouseOverButton(backButton, mouseX, mouseY)) {
                    Mix_HaltMusic();
                    currentState = GameState::MAIN_MENU;
                    resetGame();
                }
                break;
            case GameState::TUTORIAL:
                if (isMouseOverButton(backButton, mouseX, mouseY)) {
                    currentState = GameState::MAIN_MENU;
                    currentTutorialStep = 0;
                    resetGame();
                }
                else {
                    cout << elapsedSeconds;
                    if (currentTutorialStep==0)
                    {
                        if (e.button.button == SDL_BUTTON_LEFT) {
                            currentTutorialStep++;
                        }
                    }
                    if (currentTutorialStep == 1)
                    {
                        if (elapsedSeconds > 3 && e.button.button == SDL_BUTTON_LEFT) {
                            currentTutorialStep++;
                        }
                    }
                    if (currentTutorialStep == 2)
                    {
                        if (e.button.button == SDL_BUTTON_RIGHT) {
                            currentTutorialStep++;
                        }
                    }
                    if (currentTutorialStep == 3)
                    {
                        if (elapsedSeconds > 8 && e.button.button == SDL_BUTTON_RIGHT) {
                            currentTutorialStep++;
                        }
                    }
                    if (currentTutorialStep == 4)
                    {
                        if (board.isGameWon()) {
                            currentTutorialStep++;
                        }
                    }
                	if (currentTutorialStep >= tutorialSteps.size()) {
                		currentState = GameState::MAIN_MENU;
                	}
                    if (currentTutorialStep>1)
                    {
	                    int cellX = mouseX / (SCREEN_WIDTH / board.getWidth());
                    	int cellY = mouseY / (SCREEN_HEIGHT / board.getHeight());
                    	if (e.button.button == SDL_BUTTON_LEFT) {
                    		if (board.revealCell(cellX, cellY)) {
                    			Mix_PlayChannel(-1, cellRevealSound, 0);
                    		}
                    	}
                    	else if (e.button.button == SDL_BUTTON_RIGHT) {
                    		Mix_PlayChannel(-1, flagToggleSound, 0);
                    		board.toggleFlag(cellX, cellY);
                    		bool shouldAutoReveal = board.autoRevealAdjacentCells(cellX, cellY);

                    	}
                    }
                }
                break;
            default: 
                break;
            }
        }
    }
}

void Game::update() {
    if (currentState == GameState::PLAYING || currentState == GameState::TUTORIAL) {
        if (board.isGameWon()) {
        	Mix_PlayMusic(gameWinSound, 0);
            currentState = GameState::GAME_OVER;
            gameWon = true;
            gameTimer.stop();
            finalTime = elapsedSeconds;
            updateHighScores();
        }

        if (board.isGameOver()) {
            Mix_PlayMusic(gameLoseSound, 0);
            currentState = GameState::GAME_OVER;
            gameWon = false;
            board.revealAllMines();
            gameTimer.stop();
        }

        if (!gameTimer.isStarted()) {
            gameTimer.start();
        }
        elapsedSeconds = static_cast<int>(gameTimer.getElapsedTime()) / 1000;

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
    Mix_FreeMusic(backgroundMusic);
    Mix_FreeMusic(gameplayMusic);
    Mix_FreeMusic(leaderboardMusic);
    Mix_FreeChunk(cellRevealSound);
    Mix_FreeChunk(flagToggleSound);
    Mix_FreeMusic(gameWinSound);
    Mix_FreeMusic(gameLoseSound);
    Mix_FreeChunk(buttonClickSound);
    IMG_Quit();
    TTF_Quit();
    Mix_Quit();
    SDL_Quit();
}