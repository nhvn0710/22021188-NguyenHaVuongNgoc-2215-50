#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <string>
#include <map>

#include "board.h"
#include "timer.h"

using namespace std;

class Game {
public:
	
    Game();
    ~Game();
    bool initialize();
    void run();
    void resetGame();

private:
    static const int SCREEN_WIDTH = 960;
    static const int SCREEN_HEIGHT = 720;
    static const int NAVIGATE_HEIGHT = 60;
    static const int TRUE_SCREEN_HEIGHT = SCREEN_HEIGHT + NAVIGATE_HEIGHT;
    static const int BUTTON_WIDTH = 300;
    static const int BUTTON_HEIGHT = 100;
    static const int HIGHSCORE_COUNT = 15;
    static const int SLIDER_YPOSITION = 635;


    enum class GameState {
        MAIN_MENU,
        PLAYING,
        GAME_OVER,
        VIEW_HIGH_SCORES,
        TUTORIAL,
        SETTINGS,
        CREDITS,
        QUIT
    };

    struct Button {
        SDL_Rect rect{ 0, 0, 0, 0 };
        string text;
    };

    struct Difficulty {
        int width;
        int height;
        int mineCount;
        bool isCustom;
        bool operator==(const Difficulty& other) const {
            return width == other.width && height == other.height && mineCount == other.mineCount;
        }
    };

    enum class DifficultyLevel {
        EASY,
        MEDIUM,
    	HARD,
        VERYHARD,
        CUSTOM
    };

    struct HighScore {
        int time;
        DifficultyLevel difficulty;
        double weight;
    };

    struct Toggle {
        SDL_Rect rect{ 0, 0, 0, 0 };
        bool isOn = true;;
        string id;
        string label;
    };

    const Uint8 clamp(const int val, const int lower, const int upper);

	void loadFonts();

    map<string, SDL_Texture*> textures;
    SDL_Texture* loadTexture(const string& path);
    void renderTexture(const string& textureId, int x, int y, int width, int height);
    void loadTextures();
    void freeTextures();

    Mix_Music* loadMixerMusic(const string& path);
    Mix_Chunk* loadMixerChunk(const string& path);
    void loadMixers();
    Mix_Music* backgroundMusic{};
    Mix_Music* gameplayMusic{};
	Mix_Music* leaderboardMusic{};
    Mix_Chunk* cellRevealSound{}; 
    Mix_Chunk* flagToggleSound{}; 
    Mix_Music* gameWinSound{};
    Mix_Music* gameLoseSound{};
    Mix_Chunk* buttonClickSound{};

    bool initSDL();
    void setCustomDifficulty(int);
    int getDifficultySliderValue(DifficultyLevel);
    DifficultyLevel getCurrentDifficulty(int);
    
    void handleEvents();
    void update();
    void render();
    void cleanUp();

    void renderHeaderText(const string& text, int x, int y, SDL_Color color);
	void renderText(const string& text, int x, int y, SDL_Color color);
    void renderCenteredText(const string& text, int y, SDL_Color color);
    void renderButton(const Button& button);
    void renderBackButton(const Button& button);
    void renderDifficultyButton(const Button& button);
    int getSliderPosition(int value);
    void renderSlider();
    static bool isMouseOverButton(const Button& button, int mouseX, int mouseY);
    static bool isMouseOverRect(const SDL_Rect& rect, int mouseX, int mouseY);
    void setDifficulty(DifficultyLevel level);
    string difficultyToString(DifficultyLevel level);

    void renderMainMenu();
    void renderGameScreen();
    void renderGameOverScreen();
    void renderWinScreen();
    void renderHighScoreScreen();
    void renderTutorialScreen();
    void renderSettingsScreen();
    void renderCreditsScreen();

    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    GameState currentState;
    Board board;

    Button startButton;
	Button highScoresButton;
    Button settingButton;
    Button tutorialButton;
    Button quitButton;
    Button backButton;
    Button newGameButton;
    Button easyButton;
    Button mediumButton;
    Button hardButton;
    Button veryhardButton;
    Button creditsButton;

    Toggle musicToggle;
    Toggle soundToggle;
    Toggle extraLifeToggle;
    Toggle texturesToggle;

    static const Difficulty difficulties[4];
    DifficultyLevel currentDifficulty;
    Difficulty customDifficulty{};
    int sliderValue{};

    bool gameWon;
    int elapsedSeconds;
    int finalTime;
    int remainingFlags;
    Timer gameTimer;

    vector<HighScore> highScores;
    void loadHighScores();
    void updateHighScores();
    void saveHighScores();
    void displayHighScores();
    static bool compareHighScores(const HighScore& a, const HighScore& b);
    bool isNewHighScore;

    vector<string> tutorialSteps;
    int currentTutorialStep;
	void loadTutorialSteps();
    void toggleMusic() const;
    void toggleSoundEffects() const;
    void toggleExtraLife();
    void toggleTextures();
    bool altTexture;
};