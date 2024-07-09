#ifndef __CHESSBOARD_H__
#define __CHESSBOARD_H__

#include <vector>
#include <SDL2/SDL.h>

#include "GraphicPieces.h"
#include "../engineCore/chessComponents/Board.h"
#include "../engineCore/chessComponents/Move.h"

using namespace std;


struct Color {
    uint8_t r, g, b;
};

namespace Default_colors
{
    inline constexpr Color BLANCHED_ALMOND{255, 235, 205};
    inline constexpr Color BURLY_WOOD{222, 184, 135};
    inline constexpr Color SALMON{250,128,114};
};

class ChessColors {
public:
    Color light, dark, allowedMove;

    ChessColors(const Color & light, const Color & dark, const Color & allowedMove = Default_colors::SALMON) :light(light), dark(dark), allowedMove(allowedMove) {}
};


class Chessboard {
public:
    static const ChessColors DEFAULT_COLORS;

    const Board & board;

    Chessboard(SDL_Surface * surface, const ChessColors & Colors, const Board & board);

    void setAllowedMoves(const vector<Move> & moves);
    void render();

private:
    const ChessColors colors;
    const size_t size;

    SDL_Surface * surface;
    SDL_Renderer * renderer;
    GraphicPieces pieces;

    vector<uint8_t> allowedMoves;


    uint8_t getRotatedIndex(uint8_t square) const;


};

#endif  //__CHESSBOARD_H__
