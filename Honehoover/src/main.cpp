#include "game.h"
using namespace std;

int main(int argc, char* args[]) {
    Game game;
    if (!game.initialize()) {
        return 1;
    }
    game.run();
    return 0;
}
