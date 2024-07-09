#include "../src/game/RandomGame.h"
#include "../src/graphics/GraphicsGame.h"

#include <iostream>
using namespace std;

int main()
{
    RandomGame game;
    GraphicsGame graphics(game);

    graphics.startGame(1000);
    SDL_Delay(2000);

    return 0;
}
