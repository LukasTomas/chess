#include "BoardController.h"

#include "../chessComponents/Piece.h"
#include "../ChessDefinitions.h"


void BoardController::doMove( const Move & move ) {
    Piece piece = board[move.from];

    if (move.isTake)
        removePiecePosition(board[move.to], move.to);

    board[move.to] = board[move.from];
    board[move.from] = Piece();

    int castleRelativeRookStart = move.isKingCastle ? +1 : -2;
    int castleRelativeRookEnd = move.isKingCastle ? +1 : -1;
    if ( move.isCastle() ) {
        Piece rook = Piece(Rook | piece.getSide());

        board[move.to + castleRelativeRookStart] = Piece();     // remove rook from old position
        board[move.from + castleRelativeRookEnd] = rook;        // place rook on new position

        updatePiecePositions(piece, move.from, move.to);
        updatePiecePositions(rook, move.to + castleRelativeRookStart, move.from + castleRelativeRookEnd);
    } else if ( move.isPromotion() ) {
        board[move.to] = Piece(move.promotion | board[move.to].getSide());
        updatePromotingPiecePositions(piece, Piece(move.promotion | piece.getSide()), move.from, move.to);
    } else
        updatePiecePositions(piece, move.from, move.to);

}

void BoardController::undoMove( const Move & move, Piece targetPiece, Piece beforePromPiece ) {
    Piece piece = board[move.to];

    // if was take
    if (move.isTake)
        addPiecePosition(targetPiece, move.to);

    board[move.from] = board[move.to];
    board[move.to] = targetPiece;

    int catleRelativeRookStart = move.isKingCastle ? +1 : -2;
    int castleRelativeRookEnd = move.isKingCastle ? +1 : -1;
    if ( move.isCastle() ) {
        Piece rook = Piece(Rook | piece.getSide());

        board[move.to + catleRelativeRookStart] = Piece(Rook | board[move.from].getSide());
        board[move.from + castleRelativeRookEnd] = Piece();

        updatePiecePositions(piece, move.to, move.from);
        updatePiecePositions(rook, move.from + castleRelativeRookEnd, move.to + catleRelativeRookStart);
    } else if ( move.isPromotion() ) {
        board[move.from] = beforePromPiece;
        updatePromotingPiecePositions(piece, beforePromPiece, move.to, move.from);
    } else
        updatePiecePositions(piece, move.to, move.from);

}

void BoardController::reset() {
    board = Board();
    whitePieces = array<set<uint8_t>, MAX_ARRAY_SIZE>();
    blackPieces = array<set<uint8_t>, MAX_ARRAY_SIZE>();
    setInitialPos();
}

void BoardController::getPieceCounts(side_t side, array<int, MAX_ARRAY_SIZE_NO_KING> & counts) const {
    array<set<uint8_t>, MAX_ARRAY_SIZE> piecePositions = side == White ? whitePieces : blackPieces;

    bool minusKing = false;
    for ( int i = 0; i < MAX_ARRAY_SIZE; i++ ) {
        if ( pieceFromIndex(i) == King ) {
            minusKing = true;
            continue;
        }

        counts[minusKing ? i-1 : i] = piecePositions[i].size();
    }
}

array<int, MAX_ARRAY_SIZE_NO_KING> BoardController::getPieceCounts(side_t side) const {
    const array<set<uint8_t>, MAX_ARRAY_SIZE> & piecePositions = side == White ? whitePieces : blackPieces;
    array<int, MAX_ARRAY_SIZE_NO_KING> counts = {0};

    bool minusKing = false;
    for ( int i = 0; i < MAX_ARRAY_SIZE; i++ ) {
        if ( pieceFromIndex(i) == King ) {
            minusKing = true;
            continue;
        }

        counts[minusKing ? i-1 : i] = piecePositions[i].size();
    }

    return counts;
}

const array<set<uint8_t>, MAX_ARRAY_SIZE> & BoardController::getPositions(side_t side) const {
    return side == White ? whitePieces : blackPieces;
}

// PRIVATE METHODS --------------------------------------------------------------------------------------------

void BoardController::setInitialPos() {
    board[0] = Piece(Rook | White);
    board[1] = Piece(Knight | White);
    board[2] = Piece(Bishop | White);
    board[3] = Piece(Queen | White);
    board[4] = Piece(King | White);
    board[5] = Piece(Bishop | White);
    board[6] = Piece(Knight | White);
    board[7] = Piece(Rook | White);
    for ( int i = 0; i < 8; i++ )
        board[8 + i] = Piece(Pawn | White);

    board[56] = Piece(Rook | Black);
    board[57] = Piece(Knight | Black);
    board[58] = Piece(Bishop | Black);
    board[59] = Piece(Queen | Black);
    board[60] = Piece(King | Black);
    board[61] = Piece(Bishop | Black);
    board[62] = Piece(Knight | Black);
    board[63] = Piece(Rook | Black);
    for ( int i = 0; i < 8; i++ )
        board[48 + i] = Piece(Pawn | Black);

    initPiecePositions();
}

void BoardController::initPiecePositions() {
    for ( int i = 0; i < 64; i++ ) {
        Piece piece = board[i];
        if ( piece.isNone() )
            continue;

        uint8_t pieceIndex = piece.toIndex();
        set<uint8_t> & piecePositions = piece.isWhite() ? whitePieces[pieceIndex] : blackPieces[pieceIndex];
        piecePositions.insert(i);
    }
}

void BoardController::updatePiecePositions( Piece piece, uint8_t from, uint8_t to ) {
    uint8_t pieceIndex = piece.toIndex();
    set<uint8_t> & piecePositions = piece.isWhite() ? whitePieces[pieceIndex] : blackPieces[pieceIndex];
    piecePositions.erase(from);
    piecePositions.insert(to);
}

void BoardController::updatePromotingPiecePositions( Piece startingPiece, Piece promotionPiece, uint8_t from, uint8_t to ) {
    uint8_t startingPieceIndex = startingPiece.toIndex();
    set<uint8_t> & startingPiecePositions = startingPiece.isWhite() ? whitePieces[startingPieceIndex] : blackPieces[startingPieceIndex];
    startingPiecePositions.erase(from);

    int promotionPieceIndex = promotionPiece.toIndex();
    set<uint8_t> & promotingPiecePositions = startingPiece.isWhite() ? whitePieces[promotionPieceIndex] : blackPieces[promotionPieceIndex];
    promotingPiecePositions.insert(to);
}

void BoardController::removePiecePosition( Piece piece, uint8_t pos ) {
    uint8_t pieceIndex = piece.toIndex();
    set<uint8_t> & piecePositions = piece.isWhite() ? whitePieces[pieceIndex] : blackPieces[pieceIndex];
    piecePositions.erase(pos);
}

void BoardController::addPiecePosition( Piece piece, uint8_t pos ) {
    uint8_t pieceIndex = piece.toIndex();
    set<uint8_t> & piecePositions = piece.isWhite() ? whitePieces[pieceIndex] : blackPieces[pieceIndex];
    piecePositions.insert(pos);
}
