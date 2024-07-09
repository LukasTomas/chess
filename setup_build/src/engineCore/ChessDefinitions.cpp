#include "ChessDefinitions.h"

int pieceToIndex(piece_t piece) {
    return piece - 1;
}

int pieceToIndexWithoutKing(piece_t piece) {
    // assuming here that the king is the first piece
    CHECK(King == 1);

    return piece - 2;
}

piece_t pieceFromIndex(int index) {
    return index+1;
}
