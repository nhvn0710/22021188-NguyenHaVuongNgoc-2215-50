#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
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
    enum class GameState {
        MAIN_MENU,
        PLAYING,
        GAME_OVER,
        VIEW_HIGH_SCORES,
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
    };

    map<string, SDL_Texture*> textures;

    SDL_Texture* loadTexture(const string& path);
    void renderTexture(const string& textureId, int x, int y, int width, int height);
    void loadTextures();
    void freeTextures();
    
    bool initSDL();
    void setCustomDifficulty(int);
    int getDifficultySliderValue(DifficultyLevel);
    DifficultyLevel getCurrentDifficulty(int);
    void handleEvents();
    void update();
    void render();
    void cleanUp();

    void renderText(const string& text, int x, int y, SDL_Color color);
    void renderCenteredText(const string& text, int y, SDL_Color color);
    void renderButton(const Button& button);
    int getSliderPosition(int value);
    void renderSlider();
    bool isMouseOverButton(const Button& button, int mouseX, int mouseY);
    void setDifficulty(DifficultyLevel level);
    string difficultyToString(DifficultyLevel level);

    void renderMainMenu();
    void renderGameScreen();
    void renderGameOverScreen();
    void renderWinScreen();
    void renderHighScoreScreen();

    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    GameState currentState;
    Board board;

    Button startButton;
    Button quitButton;
    Button backButton;
    Button newGameButton;
    Button easyButton;
    Button mediumButton;
    Button hardButton;
    Button veryhardButton;
    Button highScoresButton;

    DifficultyLevel currentDifficulty;
    Difficulty customDifficulty;
    int sliderValue;

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

    static const int SCREEN_WIDTH = 960;
    static const int SCREEN_HEIGHT = 720;
    static const int NAVIGATE_HEIGHT = 60;
    static const int TRUE_SCREEN_HEIGHT = SCREEN_HEIGHT+NAVIGATE_HEIGHT;
    static const int BUTTON_WIDTH = 300;
    static const int BUTTON_HEIGHT = 100;
    static const Difficulty difficulties[4];

};