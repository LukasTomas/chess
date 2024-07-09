#include <catch2/catch_all.hpp>

#include "../src/engineCore/ChessDefinitions.h"
#include "../src/engineCore/chessComponents/Board.h"
#include "../src/engineCore/chessComponents/Piece.h"

TEST_CASE("Empty square")
{
    Board b;
    REQUIRE(b[0] == Piece());
}

TEST_CASE("Comparision of single piece")
{
    Board b1, b2;

    piece_t piece = Bishop;
    b1[0] = Piece(piece | White);
    b2[0] = Piece(piece | White);

    REQUIRE(b1 == b2);

    b2[0] = Piece(piece | Black);
    REQUIRE(b1 != b2);
}

TEST_CASE("comparision of many and different pieces")
{
    Board b1, b2;

    b1[0] = Piece(Bishop | White);
    b1[1] = Piece(Knight | White);
    b1[2] = Piece(Rook | White);
    b1[3] = Piece(Queen | White);
    b1[4] = Piece(King | White);
    b1[5] = Piece(Pawn | White);

    b2[0] = Piece(Bishop | White);
    b2[1] = Piece(Knight | White);
    b2[2] = Piece(Rook | White);
    b2[3] = Piece(Queen | White);
    b2[4] = Piece(King | White);
    b2[5] = Piece(Pawn | White);

    REQUIRE(b1 == b2);

    b2[5] = Piece(Pawn | Black);
    REQUIRE(b1 != b2);
}
