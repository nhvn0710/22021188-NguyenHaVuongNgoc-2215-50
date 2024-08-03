#include "game.h"

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
    case GameState::VIEW_HIGH_SCORES:
        renderHighScoreScreen();
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

    renderButton(highScoresButton);
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

void Game::renderHighScoreScreen() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
    SDL_RenderClear(renderer);

    SDL_Color titleColor = { 255, 255, 255, 255 }; 
    renderCenteredText("High Scores", 50, titleColor);


    displayHighScores();

    renderButton(backButton);

    SDL_RenderPresent(renderer);
}

