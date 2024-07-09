#include "common/Program.h"

#include "../src/common/BinaryFile.h"

#include "../src/engineCore/chessComponents/Bitboard.h"
#include "../src/engineCore/chessComponents/Board.h"

#include "../src/graphics/Chessboard.h"

#include "../src/terminal/ProgressShower.h"
#include "../src/terminal/Logging.h"

#include <SDL2/SDL.h>

#include <iostream>
#include <vector>
using namespace std;

static const string DESCRIPTION = "Analyze across all games for each position constraint satisfaction rate";
static const string STATISTICS_MAIN_DIR = "../../statistics/";
static const string BITBOARDS_FILE = "bitboards.chess";

int _main( const string & path ) {

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window * window = SDL_CreateWindow("Chessboard", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        680, 680, SDL_WINDOW_SHOWN);
    SDL_Surface * surface = SDL_GetWindowSurface(window);

    Board board;
    Chessboard chessboard(surface, Chessboard::DEFAULT_COLORS, board);

    const int waitMiliseconds = 2000;

    ifstream i(path, ios::binary);

    Bitboard bitboard;
    while ( bitboard.load(i) ) {
        cout << bitboard.toString() << endl;

        board.setFromBitboard(bitboard);
        cout << board.toString() << endl;
        cout << endl;

        chessboard.render();
        SDL_UpdateWindowSurface(window);
        SDL_Delay(waitMiliseconds);
    }

    return 0;
}

// --------------------------------------------------------------------

int main( int argc, char* argv[] ) {
    Program program(argc, argv, DESCRIPTION);
    program.helper.AddOption("bitboardsDir", "directory name containing biboards file", true);
    program.Start();

    string argDir(argv[2]);
    const string wholePath = STATISTICS_MAIN_DIR + argDir + BITBOARDS_FILE;
    if ( !binfile::FileExists(wholePath) ) {
        LOG_ERROR << "File " << wholePath << " doesn't exist" << endl;
        program.helper.ShowHelp();
        return 1;
    }

    return _main(wholePath);
}
