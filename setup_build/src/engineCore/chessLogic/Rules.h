#ifndef __RULES_H__
#define __RULES_H__

#include "../ChessDefinitions.h"
#include "../chessComponents/Piece.h"
#include "../chessComponents/Board.h"
#include "../chessComponents/Move.h"

#include <array>
#include <vector>
#include <map>

namespace DirectionOffset {
    enum Value {
        North = 8,
        South = -8,
        West = -1,
        East = +1,

        NorthWest = 7,
        NorthEast = 9,
        SouthWest = -9,
        SouthEast = -7,
    };

    const size_t N_OFFSETS = 8;
    const array<int, N_OFFSETS> offsetsArray = {North, South, West, East, NorthWest, NorthEast, SouthWest, SouthEast};
};

using namespace DirectionOffset;
using namespace chessboardSize;


class Rules {
public:

    static const int WHITE_STARTING_ROW = 0;
    static const int BLACK_STARTING_ROW = LINE_SQUARES - 1;

    static const vector< pair<int, int> > KNIGHT_MOVES;

    static bool canInThisDirection(uint8_t square, int direction);

    struct PieceInfo {
        piece_t piece;
        bool infMoves;
        vector<int> directions;
    };


    Rules( const Board & board );

    void getPossibleMoves(vector<Move> & possibleMoves, uint8_t startingSquare) const;

private:

    class UnknownName {
    public:

        UnknownName(const Rules & supervisor, uint8_t startingSquare);

        void getPossibleMoves(vector<Move> & possibleMoves);
        void getSpecialMoves( vector<Move> & moves );


    private:
        const Rules & supervisor;

        const uint8_t startingSquare;
        const PieceInfo & pieceInfo;
        const map<int, int> & maxMoves;

        // NEW NOTATION - members which should be const, but can't because value must be computed in constructor
        //                and can not be in initializer list ( or for clarity ) FROM NOW ON START WITH UNDESCORE -
        piece_t _piece;
        side_t _side;

        int _enemyLastRow;

        void addMove( vector<Move> & moves, uint8_t from, uint8_t to );

    };


    const Board & board;
    bool isCheck = false;

    array<PieceInfo, PieceType::Max> whitePieceInfos, blackPieceInfos;

    // 2d array - each square has 8 possible offsets
    array<map<int, int>, SQUARES> precomputedMaxMoves;

    //called in constructor
    void setPieceInfos();

    const PieceInfo & getPieceInfo(Piece piece) const;
};



#endif
