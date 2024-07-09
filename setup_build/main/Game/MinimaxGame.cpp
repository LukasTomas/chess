#include "../src/game/MinimaxGame.h"
#include "../src/graphics/GraphicsGame.h"

#include <iostream>
using namespace std;

int main()
{
    MinimaxGame game;
    GraphicsGame graphics(game);

    graphics.startGame(1000);
    SDL_Delay(2000);

    //game.game.saveMoves("moves.txt");

    return 0;
}
