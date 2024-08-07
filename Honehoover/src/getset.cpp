#include "game.h"


const Game::Difficulty Game::difficulties[] = {
    { 12, 9, 4 },
    { 20, 15, 36 },
    { 24, 18, 90 },
    { 30, 24, 226 }
};

void Game::loadTutorialSteps()
{
    tutorialSteps = {
        "Welcome to the Honehoover tutorial!",
        "Left-click on a cell to reveal it. Try now!",
        "Right-click to place on suspected mine.",
        "Right-click on opened cell for a quick clear.",
        "Clear all cells without stumbling on a mine!"
    };
    currentTutorialStep = 0;
}


const Uint8 Game::clamp(const int val, const int lower, const int upper) {
    if (val < lower) return lower;
    else if (val > upper) return upper;
    else return val;
}

int Board::getFlagCount() const {
    int count = 0;
    for (const auto& row : cells) {
        for (const auto& cell : row) {
            if (cell.isFlagged()) {
                count++;
            }
        }
    }
    return count;
}

int Board::getWidth() const {
    return width;
}

int Board::getHeight() const {
    return height;
}

int Board::getMineCount() const { return mineCount; }

bool Board::isLifeCheat() const
{
    return extraLife;
}

void Board::changeLifeCheat()
{
    extraLife = !extraLife;
}

bool Board::hasExtraLife() const { return lifeCount; }

void Board::useExtraLife() { lifeCount--; }

void Board::restoreExtraLife() { lifeCount=lifeMax; }


bool Game::isMouseOverButton(const Button& button, int mouseX, int mouseY) {
    return (mouseX >= button.rect.x && mouseX <= button.rect.x + button.rect.w &&
        mouseY >= button.rect.y && mouseY <= button.rect.y + button.rect.h);
}

bool Game::isMouseOverRect(const SDL_Rect& rect, int mouseX, int mouseY) {
    return (mouseX >= rect.x && mouseX <= rect.x + rect.w &&
        mouseY >= rect.y && mouseY <= rect.y + rect.h);
}

int Game::getSliderPosition(int value) {
    const int sliderMaxPos = SCREEN_WIDTH - 60;
    const int sliderStart = 20;

    int easyPos = sliderStart + (getDifficultySliderValue(DifficultyLevel::EASY) * sliderMaxPos / 100);
    int mediumPos = sliderStart + (getDifficultySliderValue(DifficultyLevel::MEDIUM) * sliderMaxPos / 100);
    int hardPos = sliderStart + (getDifficultySliderValue(DifficultyLevel::HARD) * sliderMaxPos / 100);
    int veryhardPos = sliderStart + (getDifficultySliderValue(DifficultyLevel::VERYHARD) * sliderMaxPos / 100);

    int customPos = sliderStart + (value * sliderMaxPos / 100);
    if (abs(customPos - easyPos) < 5) customPos = easyPos;
    if (abs(customPos - mediumPos) < 5) customPos = mediumPos;
    if (abs(customPos - hardPos) < 5) customPos = hardPos;
    if (abs(customPos - veryhardPos) < 5) customPos = veryhardPos;

    return customPos;
}

int Game::getDifficultySliderValue(DifficultyLevel level) {
    switch (level) {
    case DifficultyLevel::EASY:
        return 0;
    case DifficultyLevel::MEDIUM:
        return 32;
    case DifficultyLevel::HARD:
        return 50;
    case DifficultyLevel::VERYHARD:
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
    else if (abs(sliderVal - getDifficultySliderValue(DifficultyLevel::HARD)) < 5) {
        return DifficultyLevel::HARD;
    }
    else if (abs(sliderVal - getDifficultySliderValue(DifficultyLevel::VERYHARD)) < 5) {
        return DifficultyLevel::VERYHARD;
    }
    return DifficultyLevel::CUSTOM;
}

string Game::difficultyToString(DifficultyLevel level) {
    switch (level) {
    case DifficultyLevel::EASY:
        return "Easy";
    case DifficultyLevel::MEDIUM:
        return "Medium";
    case DifficultyLevel::HARD:
        return "Hard";
    case DifficultyLevel::VERYHARD:
        return "VeryHard";
    case DifficultyLevel::CUSTOM:
        return "Custom";
    default:
        return "Unknown Difficulty";
    }
}

void Game::setDifficulty(DifficultyLevel level) {
    switch (level) {
    case DifficultyLevel::EASY:
        board = Board(difficulties[0].width, difficulties[0].height, difficulties[0].mineCount);
        break;
    case DifficultyLevel::MEDIUM:
        board = Board(difficulties[1].width, difficulties[1].height, difficulties[1].mineCount);
        break;
    case DifficultyLevel::HARD:
        board = Board(difficulties[2].width, difficulties[2].height, difficulties[2].mineCount);
        break;
    case DifficultyLevel::VERYHARD:
        board = Board(difficulties[3].width, difficulties[3].height, difficulties[3].mineCount);
        break;
    case DifficultyLevel::CUSTOM:
        board = Board(customDifficulty.width, customDifficulty.height, customDifficulty.mineCount);
        if (customDifficulty == difficulties[0]) { //0
            currentDifficulty = DifficultyLevel::EASY;
        }
        else if (customDifficulty == difficulties[1]) { //32
            currentDifficulty = DifficultyLevel::MEDIUM;
        }
        else if (customDifficulty == difficulties[2]) { //50
            currentDifficulty = DifficultyLevel::HARD;
        }
        else if (customDifficulty == difficulties[3]) { //72
            currentDifficulty = DifficultyLevel::VERYHARD;
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
    int width = difficulties[0].width + sliderVal * 5 / 20;
    int height = difficulties[0].height + sliderVal * 5 / 24;
    if (sliderVal == 100)
    {
        width = 40;
        height = 30;
    }
    int mineCount = static_cast<int>(difficulties[0].mineCount + sliderVal * (0.5 + (static_cast<double>(sliderVal) * sliderVal) / 2000));
    customDifficulty = { width, height, mineCount, true };
}

void Game::toggleMusic() const
{
    if (musicToggle.isOn) {
        Mix_VolumeMusic(MIX_MAX_VOLUME);
    }
    else {
        Mix_VolumeMusic(0);
    }
}

void Game::toggleSoundEffects() const
{
    if (soundToggle.isOn) {
        Mix_Volume(-1, MIX_MAX_VOLUME);
    }
    else {
        Mix_Volume(-1, 0);
    }
}

void Game::toggleExtraLife() {
    board.changeLifeCheat();
}

void Game::toggleTextures() {
    //useAlternativeTextures = texturesToggle.isOn;
}
