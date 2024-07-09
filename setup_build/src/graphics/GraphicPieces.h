#ifndef __GRAPHICPIECES_H__
#define __GRAPHICPIECES_H__

#include "../engineCore/chessComponents/Piece.h"
#include "../engineCore/PieceIterator.h"

#include <iostream>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


#include <filesystem>
using namespace std;
using namespace filesystem;


class GraphicPieces {
public:

    GraphicPieces();

    const map<piece_t, SDL_Surface *> & operator[](side_t side) const;


private:
    static const string SRC_DIR_NAME;
    static const string PIECES_DIR_NAME;

    static const pair<side_t, string> SIDE_NAMES[2];

    map<piece_t, SDL_Surface *> whitePieces;
    map<piece_t, SDL_Surface *> blackPieces;


    path getPiecesDirPath() const;

};

#endif  //__GRAPHICPIECES_H__
