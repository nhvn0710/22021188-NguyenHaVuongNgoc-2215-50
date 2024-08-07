#pragma once
#include <SDL_stdinc.h>
#include <SDL_timer.h>
using namespace std;

class Timer {
public:
    Timer() : startTicks(0), started(false) {}

    void start() {
        started = true;
        startTicks = SDL_GetTicks();
    }
    void stop();
    Uint32 getElapsedTime() const;
    bool isStarted() const { return started; }

private:
    Uint32 startTicks;
    bool started;
};