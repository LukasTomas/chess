#ifndef _CHESSDEFINITIONS_H__
#define _CHESSDEFINITIONS_H__

#include <map>
#include <string>
#include <cstddef>
#include <cstdint>

using namespace std;

#ifdef DEBUG \
    #define CHECK( expr ) \
        if (!(expr)) \
            throw std::runtime_error("CHECK failed: " #(expr));
#else
    #define CHECK( expr ) do { } while ( 0 )
#endif

enum PieceType {
    None = 0,
    King = 1,
    Pawn = 2,
    Knight = 3,
    Bishop = 4,
    Rook = 5,
    Queen = 6,
    Max = 7
};

enum Side {
    No_Side = 0,
    White = 8,
    Black = 16,
};

typedef uint8_t piece_t;
typedef uint8_t side_t;

const map<piece_t, string> PIECE_NAMES = {
        {None, "no-piece"},
        {King, "king"},
        {Queen, "queen"},
        {Rook, "rook"},
        {Bishop, "bishop"},
        {Knight, "knight"},
        {Pawn, "pawn"}
};

const map<side_t, string> SIDE_NAMES = {
    {No_Side, "no-side"},
    {White, "white"},
    {Black, "black"}
};

int pieceToIndex(piece_t piece);
int pieceToIndexWithoutKing(piece_t piece);

piece_t pieceFromIndex(int index);


/* FOR ARRAY OF PIECES */
const int MAX_ARRAY_SIZE = PieceType::Max - 1;
const int MAX_ARRAY_SIZE_NO_KING = MAX_ARRAY_SIZE - 1;

namespace chessboardSize {
    static const size_t LINE_SQUARES = 8;
    static const size_t SQUARES = LINE_SQUARES * LINE_SQUARES;
};



#endif  // _CHESSDEFINITIONS_H__
