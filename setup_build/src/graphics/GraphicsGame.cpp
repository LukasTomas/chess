#include "GraphicsGame.h"

GraphicsGame::GraphicsGame( AbstractGame & game ) : game(game) {
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("Chessboard", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

    SDL_Surface * surface = SDL_GetWindowSurface(window);

    chessboard = new Chessboard(surface, Chessboard::DEFAULT_COLORS, game.getBoard());

    chessboard->render();
    SDL_UpdateWindowSurface(window);
}


GraphicsGame::~GraphicsGame() {
    delete chessboard;
    SDL_DestroyWindow(window);
    SDL_Quit();
}


void GraphicsGame::startGame( int waitMilisecond ) {
    bool quit = false;
    SDL_Event event;
    while ( !quit ) {
        while ( SDL_PollEvent(&event) ) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
            }
        }

        // break from pollevent
        if ( quit )
            break;


        SDL_Delay(waitMilisecond);

        quit = game.makeMove() == false;
        chessboard->render();
        SDL_UpdateWindowSurface(window);
    }
}
