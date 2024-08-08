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
    case GameState::TUTORIAL:
        renderTutorialScreen();
        break;
    case GameState::SETTINGS:
        renderSettingsScreen();
        break;
    case GameState::CREDITS:
        renderCreditsScreen();
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

void Game::renderHeaderText(const string& text, int x, int y, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    int width, height;
    SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
    SDL_Rect dstRect = { x, y, 2*width, 2*height };

    SDL_RenderCopy(renderer, texture, nullptr, &dstRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
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
    SDL_Texture* buttonBgTexture = textures["btton"];
    if (buttonBgTexture) {
        SDL_RenderCopy(renderer, buttonBgTexture, nullptr, &button.rect);
    }
    else {
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        SDL_RenderFillRect(renderer, &button.rect);
    }
    SDL_Color textColor = { 0, 0, 0, 255 };
    renderText(button.text, button.rect.x + 68, button.rect.y + 35, textColor);
}

void Game::renderBackButton(const Button& button) {
    SDL_Texture* buttonBgTexture = textures["bttn2"];
    if (buttonBgTexture) {
        SDL_RenderCopy(renderer, buttonBgTexture, nullptr, &button.rect);
    }
    else {
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        SDL_RenderFillRect(renderer, &button.rect);
    }
    SDL_Color textColor = { 0, 0, 0, 255 };
}

void Game::renderDifficultyButton(const Button& button) {
    SDL_Texture* buttonBgTexture = textures["bttn1"];
    if (buttonBgTexture) {
        SDL_RenderCopy(renderer, buttonBgTexture, nullptr, &button.rect);
    }
    else {
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        SDL_RenderFillRect(renderer, &button.rect);
    }
    SDL_Color textColor = { 0, 0, 0, 255 };
}

void Game::renderSlider() {
    SDL_Rect sliderBar = { 20, SLIDER_YPOSITION+10, SCREEN_WIDTH - 40, 10 };
    SDL_Rect sliderHandle = { getSliderPosition(sliderValue), SLIDER_YPOSITION, 20, 30 };

    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderFillRect(renderer, &sliderBar);

    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderFillRect(renderer, &sliderHandle);

    SDL_Color textColor = { 0, 0, 0, 255 };
    string difficultyText;
    DifficultyLevel tmp = getCurrentDifficulty(sliderValue);
    if (tmp==DifficultyLevel::CUSTOM) {
        difficultyText = to_string(customDifficulty.width) + "x" + to_string(customDifficulty.height) + " " + to_string(customDifficulty.mineCount) + " mines";
    }
    else {
        if (tmp==DifficultyLevel::EASY)
        {
            difficultyText = to_string(difficulties[0].width) + "x" + to_string(difficulties[0].height) + " " + to_string(difficulties[0].mineCount) + " mines";
        } else
        if (tmp==DifficultyLevel::MEDIUM){
            difficultyText = to_string(difficulties[1].width) + "x" + to_string(difficulties[1].height) + " " + to_string(difficulties[1].mineCount) + " mines";
        } else 
            if (tmp == DifficultyLevel::HARD) {
                difficultyText = to_string(difficulties[2].width) + "x" + to_string(difficulties[2].height) + " " + to_string(difficulties[2].mineCount) + " mines";
            }
            else
        {
            difficultyText = to_string(difficulties[3].width) + "x" + to_string(difficulties[3].height) + " " + to_string(difficulties[3].mineCount) + " mines";
        }
    }
    double tmpn = sliderHandle.x * ((static_cast<double>(100) - sliderValue) / 500 + 0.8) + sliderHandle.w / static_cast<double>(2) - 28;
    renderText(difficultyText, static_cast<int>(tmpn), sliderHandle.y - 40, textColor);
}



void Game::renderMainMenu() {
    SDL_Texture* backgroundTexture = textures["mmnbg"];
    if (backgroundTexture) {
        SDL_Rect renderQuad = { 0, 0, SCREEN_WIDTH, TRUE_SCREEN_HEIGHT };
        SDL_RenderCopy(renderer, backgroundTexture, nullptr, &renderQuad);
    }
    else {
        SDL_SetRenderDrawColor(renderer, 0xAA, 0xFF, 0xDD, 255);
        SDL_RenderClear(renderer);
    }
    if (Mix_PlayingMusic() == 0) {
        Mix_PlayMusic(backgroundMusic, -1);
    }
    

    renderButton(startButton);
    renderButton(tutorialButton);
    renderButton(highScoresButton);
	renderButton(settingButton);
    renderButton(quitButton);
    renderDifficultyButton(easyButton);
    renderDifficultyButton(mediumButton);
    renderDifficultyButton(hardButton);
    renderDifficultyButton(veryhardButton);
    renderSlider();

    SDL_Color titleColor = { 155, 225, 155, 255 };
    renderHeaderText("Honehoover", SCREEN_WIDTH / 2 - 180, 100, titleColor);
    
    SDL_Color difficultyColor = { 255, clamp(255 - sliderValue * 2, 0, 255), clamp(255 - sliderValue * 2, 0, 255), 255 };
    string difficultyText = "Difficulty: " + difficultyToString(currentDifficulty);
    renderCenteredText(difficultyText, SLIDER_YPOSITION+60, difficultyColor);
}

void Game::renderGameScreen() {
    SDL_Texture* backgroundTexture = textures["mmnbg"];
    if (backgroundTexture) {
        SDL_Rect renderQuad = { 0, 0, SCREEN_WIDTH, TRUE_SCREEN_HEIGHT };
        SDL_RenderCopy(renderer, backgroundTexture, nullptr, &renderQuad);
    }
    else {
        SDL_SetRenderDrawColor(renderer, 0xAA, 0xFF, 0xDD, 255);
        SDL_RenderClear(renderer);
    }
    board.render(renderer, font, SCREEN_WIDTH, SCREEN_HEIGHT, textures);
    renderBackButton(backButton);

    if (Mix_PlayingMusic() == 0) {
        Mix_PlayMusic(gameplayMusic, -1);
    }

    SDL_Color textColor = { 0, 150, 225, 255 };
    string timeText = "Time: " + to_string(elapsedSeconds) + " s";
    renderText(timeText, SCREEN_WIDTH - 360, TRUE_SCREEN_HEIGHT - 40, textColor);

    string flagText = "Flags: " + to_string(remainingFlags);
    renderText(flagText, SCREEN_WIDTH - 160, TRUE_SCREEN_HEIGHT - 40, textColor);
}

void Game::renderGameOverScreen() {
    SDL_Texture* backgroundTexture = textures["govbg"];
    if (backgroundTexture) {
        SDL_Rect renderQuad = { 0, 0, SCREEN_WIDTH, TRUE_SCREEN_HEIGHT };
        SDL_RenderCopy(renderer, backgroundTexture, nullptr, &renderQuad);
    }
    else {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
    }

    SDL_Color textColor;
    if (altTexture)
    {
        textColor = { 255, 255, 255, 255 };
    }
    else {
        textColor = { 0, 0, 0, 255 };
    }
    renderText("Game Over!", SCREEN_WIDTH / 2 - 100, 100, textColor);
    renderText("You Lost!", SCREEN_WIDTH / 2 - 80, 150, textColor);
    renderButton(newGameButton);
    renderBackButton(backButton);
}

void Game::renderWinScreen() {
    SDL_Texture* backgroundTexture = textures["govbg"];
    if (backgroundTexture) {
        SDL_Rect renderQuad = { 0, 0, SCREEN_WIDTH, TRUE_SCREEN_HEIGHT };
        SDL_RenderCopy(renderer, backgroundTexture, nullptr, &renderQuad);
    }
    else {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
    }

    SDL_Color textColor;
    if (altTexture)
    {
        textColor = { 255, 255, 255, 255 };
    }
    else {
        textColor = { 0, 0, 0, 255 };
    }
    string timeText = "Time: " + to_string(finalTime) + " seconds";
    renderText(timeText, SCREEN_WIDTH / 2 - 120, 200, textColor);
    renderButton(newGameButton);
    renderBackButton(backButton);
    string victoryMessage = "You cleared all mines! Well done!";
    renderCenteredText(victoryMessage, TRUE_SCREEN_HEIGHT / 2 + 50, textColor);

    if (isNewHighScore) {
        textColor = { 255, 255, 0, 255 };
        string highScoreText = "New High Score!";
        renderCenteredText(highScoreText, TRUE_SCREEN_HEIGHT / 2 + 100, textColor);
    }
}

void Game::renderHighScoreScreen() {
    SDL_Texture* backgroundTexture = textures["ldbbg"];
    if (backgroundTexture) {
        SDL_Rect renderQuad = { 0, 0, SCREEN_WIDTH, TRUE_SCREEN_HEIGHT };
        SDL_RenderCopy(renderer, backgroundTexture, nullptr, &renderQuad);
    }
    else {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
    }

    SDL_Color titleColor = { 255, 255, 255, 255 }; 
    renderCenteredText("High Scores", 50, titleColor);

    if (Mix_PlayingMusic() == 0) {
        Mix_PlayMusic(leaderboardMusic, -1);
    }

    displayHighScores();
    renderBackButton(backButton);
    SDL_RenderPresent(renderer);
}

void Game::renderTutorialScreen() {
    SDL_Texture* backgroundTexture = textures["mmnbg"];
    if (backgroundTexture) {
        SDL_Rect renderQuad = { 0, 0, SCREEN_WIDTH, TRUE_SCREEN_HEIGHT };
        SDL_RenderCopy(renderer, backgroundTexture, nullptr, &renderQuad);
    }
    else {
        SDL_SetRenderDrawColor(renderer, 0xAA, 0xFF, 0xDD, 255);
        SDL_RenderClear(renderer);
    }
    board.render(renderer, font, SCREEN_WIDTH, SCREEN_HEIGHT, textures);
    SDL_Color textColor;
    if (altTexture)
    {
        textColor = { 255, 255, 255, 255 };
    }
    else {
        textColor = { 0, 0, 0, 255 };
    }
    string tutorialText = tutorialSteps[currentTutorialStep];
    renderText(tutorialText, BUTTON_WIDTH/2+10, TRUE_SCREEN_HEIGHT - 40, textColor);

    string stepCounterText = "Step " + to_string(currentTutorialStep + 1) + " of " + to_string(tutorialSteps.size());
    //renderText(stepCounterText, (SCREEN_WIDTH - BUTTON_WIDTH) / 2, (SCREEN_HEIGHT / 2) + 50, textColor);

    renderBackButton(backButton);
    
    textColor = { 0, 150, 225, 255 };
    string timeText = "Time: " + to_string(elapsedSeconds) + " s";

    string flagText = "Flags: " + to_string(remainingFlags);
    renderText(flagText, SCREEN_WIDTH - 140, TRUE_SCREEN_HEIGHT - 40, textColor);

    SDL_RenderPresent(renderer);
}

void Game::renderSettingsScreen() {
    SDL_Texture* backgroundTexture = textures["mmnbg"];
    if (backgroundTexture) {
        SDL_Rect renderQuad = { 0, 0, SCREEN_WIDTH, TRUE_SCREEN_HEIGHT };
        SDL_RenderCopy(renderer, backgroundTexture, nullptr, &renderQuad);
    }
    else {
        SDL_SetRenderDrawColor(renderer, 0xAA, 0xFF, 0xDD, 255);
        SDL_RenderClear(renderer);
    }

    auto renderToggle = [&](const Toggle& toggle) {
        string toggleTextureId = toggle.isOn ? toggle.id + "_on" : toggle.id + "_off";
        SDL_Texture* toggleTexture = textures[toggleTextureId];
        SDL_RenderCopy(renderer, toggleTexture, NULL, &toggle.rect);
        };

    renderToggle(musicToggle);
    renderToggle(soundToggle);
    renderToggle(extraLifeToggle);
    renderToggle(texturesToggle);

    renderButton(creditsButton);
    
    renderBackButton(backButton);
}

void Game::renderCreditsScreen() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Color textColor = { 255, 255, 255, 255 };
    string creditsText0 = "Honehoover is a Minesweeper - inspired game";
	string creditsText01 = "implemented in Cpp using the SDL2 library";
    string creditsText1 = "Game developed by nhvn0710";
    string creditsText2 = "Music by nhvn0710 and from freesound.org";
    string creditsText3 = "Art by nhvn0710 and from craftpix.net";
    string creditsText4 = "Special Thanks to everyone at UET";

    renderCenteredText(creditsText0, SCREEN_HEIGHT / 4, textColor);
    renderCenteredText(creditsText01, SCREEN_HEIGHT / 4 + BUTTON_HEIGHT/2, textColor);
    renderCenteredText(creditsText1, SCREEN_HEIGHT / 4 + BUTTON_HEIGHT * 3 / 2, textColor);
    renderCenteredText(creditsText2, SCREEN_HEIGHT / 4 + BUTTON_HEIGHT * 4 / 2, textColor);
    renderCenteredText(creditsText3, SCREEN_HEIGHT / 4 + BUTTON_HEIGHT * 5 / 2, textColor);
    renderCenteredText(creditsText4, SCREEN_HEIGHT / 4 + BUTTON_HEIGHT * 6 / 2, textColor);

    renderBackButton(backButton);
}

