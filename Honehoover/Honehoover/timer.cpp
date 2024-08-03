#include "timer.h"

void Timer::stop() {
    started = false;
    startTicks = 0;
}

Uint32 Timer::getElapsedTime() const {
    return started ? (SDL_GetTicks() - startTicks) : 0;
}