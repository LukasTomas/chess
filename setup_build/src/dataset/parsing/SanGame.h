#pragma once

#include "../../engineCore/ChessDefinitions.h"
#include "../../engineCore/chessLogic/Game.h"

class SanGame {
public:
    Game game;

    SanGame() : game(false, false) { }

    bool makeMove( string san );

private:
    const map<char, piece_t> PIECE_LETTER = {
        {'K', King},
        {'Q', Queen},
        {'R', Rook},
        {'B', Bishop},
        {'N', Knight}
    };


    void makeCastle(bool isKingCastle);
    int getColumnNumber(char columnLetter);
    uint8_t toCoordinates( string san );
    piece_t letterToPiece( char letter );
    uint8_t getFromSquare(piece_t piece, uint8_t to, char columnLetter);

};
