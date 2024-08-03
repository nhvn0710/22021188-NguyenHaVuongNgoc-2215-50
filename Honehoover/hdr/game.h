#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

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
        QUIT
    };

    struct Button {
        SDL_Rect rect{ 0, 0, 0, 0 };
        string text;
    };

    struct Difficulty {
        const int width;
        const int height;
        const int mineCount;
    };

    enum class DifficultyLevel {
        EASY,
        MEDIUM,
        HARD
    };

    struct HighScore {
        int time;
        DifficultyLevel difficulty;
    };



    bool initSDL();
    void handleEvents();
    void update();
    void render();
    void cleanUp();

    void renderText(const string& text, int x, int y, SDL_Color color);
    void renderCenteredText(const string& text, int y, SDL_Color color);
    void renderButton(const Button& button);
    bool isMouseOverButton(const Button& button, int mouseX, int mouseY);
    void setDifficulty(DifficultyLevel level);

    void renderMainMenu();
    void renderGameScreen();
    void renderGameOverScreen();
    void renderWinScreen();

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

    DifficultyLevel currentDifficulty;

    bool gameWon;
    int elapsedSeconds;
    int finalTime;
    int remainingFlags;
    Timer gameTimer;

    vector<HighScore> highScores;
    void loadHighScores();
    void updateHighScores();
    void saveHighScores() const;
    void displayHighScores();
    static bool compareHighScores(const HighScore& a, const HighScore& b);

    static const int SCREEN_WIDTH = 960;
    static const int SCREEN_HEIGHT = 960;
    static const int BUTTON_WIDTH = 200;
    static const int BUTTON_HEIGHT = 50;
    static const Difficulty difficulties[3];

};