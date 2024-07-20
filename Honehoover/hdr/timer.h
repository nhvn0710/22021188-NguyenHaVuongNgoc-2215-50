#include <SDL_stdinc.h>
#include <SDL_timer.h>
using namespace std;

class Timer {
private:
    Uint32 startTicks;
    bool started;

public:
    Timer() : startTicks(0), started(false) {}

    void start() {
        started = true;
        startTicks = SDL_GetTicks();
    }

    void stop() {
        started = false;
        startTicks = 0;
    }

    Uint32 getElapsedTime() {
        if (started) {
            return SDL_GetTicks() - startTicks;
        }
        return 0;
    }

    bool isStarted() const {
        return started;
    }
};