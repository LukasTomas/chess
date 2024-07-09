#ifndef __BOARDCONTROLLER_H__
#define __BOARDCONTROLLER_H__

#include <array>
#include <set>

#include "../ChessDefinitions.h"
#include "../chessComponents/Board.h"
#include "../chessComponents/Move.h"

class BoardController {
public:

    Board board;

    BoardController() {
        setInitialPos();
    }

    BoardController( Board & board ) : board(board) { }

    void doMove( const Move & move );
    void undoMove( const Move & move, Piece targetPiece = Piece(), Piece beforePromPiece = Piece() );

    void reset();

    void getPieceCounts(side_t side, array<int, MAX_ARRAY_SIZE_NO_KING> & counts) const;
    array<int, MAX_ARRAY_SIZE_NO_KING> getPieceCounts(side_t side) const;

    const array<set<uint8_t>, MAX_ARRAY_SIZE> & getPositions(side_t side) const;

private:

    // positions of each piece
    array<set<uint8_t>, MAX_ARRAY_SIZE> whitePieces, blackPieces;

    void setInitialPos();
    void initPiecePositions();

    void updatePiecePositions( Piece piece, uint8_t from, uint8_t to );
    void updatePromotingPiecePositions( Piece startingPiece, Piece promotionPiece, uint8_t from, uint8_t to );
    void removePiecePosition( Piece piece, uint8_t pos );
    void addPiecePosition( Piece piece, uint8_t pos );

};

#endif
