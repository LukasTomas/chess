#ifndef __GRAPHICSGAME_H__
#define __GRAPHICSGAME_H__

#include <SDL2/SDL.h>

#include "../game/AbstractGame.h"
#include "Chessboard.h"

class GraphicsGame {
public:
    static const int WIDTH = 680;
    static const int HEIGHT = 680;

    GraphicsGame( AbstractGame & game );
    ~GraphicsGame();

    void startGame( int waitMiliseconds );

private:
    AbstractGame & game;
    // Chessboard class doesn't have move constructor, thats why pointer
    Chessboard * chessboard;
    
    SDL_Window * window = NULL;

};


#endif