#include "game.h"

bool Game::isMouseOverButton(const Button& button, int mouseX, int mouseY) {
    return (mouseX >= button.rect.x && mouseX <= button.rect.x + button.rect.w &&
        mouseY >= button.rect.y && mouseY <= button.rect.y + button.rect.h);
}

int Game::getSliderPosition(int value) {
    const int sliderMaxPos = SCREEN_WIDTH - 60;
    const int sliderStart = 20;

    int easyPos = sliderStart + (getDifficultySliderValue(DifficultyLevel::EASY) * sliderMaxPos / 100);
    int mediumPos = sliderStart + (getDifficultySliderValue(DifficultyLevel::MEDIUM) * sliderMaxPos / 100);
    int hardPos = sliderStart + (getDifficultySliderValue(DifficultyLevel::HARD) * sliderMaxPos / 100);
    int veryhardPos = sliderStart + (getDifficultySliderValue(DifficultyLevel::VERYHARD) * sliderMaxPos / 100);

    int customPos = sliderStart + (value * sliderMaxPos / 100);
    if (abs(customPos - easyPos) < 10) customPos = easyPos;
    if (abs(customPos - mediumPos) < 10) customPos = mediumPos;
    if (abs(customPos - hardPos) < 10) customPos = hardPos;
    if (abs(customPos - veryhardPos) < 10) customPos = veryhardPos;

    return customPos;
}

const Game::Difficulty Game::difficulties[] = {
    { 12, 9, 4 },
    { 20, 15, 20 },
    { 24, 18, 90 },
    { 30, 24, 184 }
};

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
