#pragma once

#include "../../pgn/PGN.hpp"

#include <iostream>
#include <fstream>
using namespace std;

//#define SHOW_CHESSBOARD
#ifdef SHOW_CHESSBOARD
#include "../../graphics/Chessboard.h"
#include <SDL2/SDL.h>
#endif

#include "SanGame.h"

class SanParser {
public:
    const string RESOURCES_DIRECTORY_NAME = "chess_resources/";

    const string PGN_STARTING_NAME = "games_";
    const string PGN_FILE_DATE = "2017-02";

    const string PARSED_GAMES_FILENAME_BASE_NAME = "parsed_games_";
    const string PARSED_GAMES_FILENAME_EXTENSION = ".chess";


    SanParser();
    ~SanParser();

    void parseAll();
    void saveNext( SanGame & sanGame );

private:
    ofstream o;
    pgnp::PGN pgn;

#ifdef SHOW_CHESSBOARD
    SDL_Window * window;
    SDL_Surface * surface;
#endif

};
