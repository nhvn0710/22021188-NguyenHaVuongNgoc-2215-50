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

private:
    enum class GameState {
        MAIN_MENU,
        PLAYING,
        GAME_OVER,
        QUIT
    };

    struct Button {
        SDL_Rect rect;
        string text;
    };

    bool initSDL();
    void handleEvents();
    void update();
    void render();
    void cleanUp();

    void renderText(const string& text, int x, int y, SDL_Color color);
    void renderButton(const Button& button);
    bool isMouseOverButton(const Button& button, int mouseX, int mouseY);

    void renderMainMenu();
    void renderGameScreen();
    void renderGameOverScreen();

    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    GameState currentState;
    Board board;

    Button startButton;
    Button quitButton;
    Button backButton;
    Button newGameButton;

    bool gameWon;
    int elapsedSeconds;
    int remainingFlags;
    Timer gameTimer;

    static const int SCREEN_WIDTH = 640;
    static const int SCREEN_HEIGHT = 480;
    static const int BUTTON_WIDTH = 200;
    static const int BUTTON_HEIGHT = 50;
};