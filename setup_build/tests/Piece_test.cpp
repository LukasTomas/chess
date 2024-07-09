#include <catch2/catch_all.hpp>

#include "../src/engineCore/ChessDefinitions.h"
#include "../src/engineCore/PieceIterator.h"
#include "../src/engineCore/chessComponents/Piece.h"

TEST_CASE("Piece Class Test") {

    SECTION("Test default constructor") {
        Piece piece;
        REQUIRE(piece.getPieceType() == PieceType::None);
        REQUIRE(piece.getSide() == Side::No_Side);
        REQUIRE(piece.isNone());
    }

    SECTION("Test constructor with colorPiece") {
        uint8_t colorPiece = Side::Black | PieceType::Rook;
        Piece piece(colorPiece);
        REQUIRE(piece.getPieceType() == PieceType::Rook);
        REQUIRE(piece.getSide() == Side::Black);
        REQUIRE(!piece.isNone());
    }

    SECTION("Test constructor all combinations") {
        for (PieceIterator it(false); it.next(); ) {
            for ( side_t side : {Side::White, Side::Black} ) {
                Piece piece(it.current | side);
                REQUIRE(piece.getPieceType() == it.current);
                REQUIRE(piece.getSide() == side);
                REQUIRE(!piece.isNone());
            }
        }
    }

    SECTION("Test constructor with index and side") {
        for (int pieceIndex = 0; pieceIndex < PieceType::Max; pieceIndex++) {
            for ( side_t side : {Side::White, Side::Black} ) {
                Piece piece(pieceIndex, side);
                REQUIRE(piece.getPieceType() == pieceFromIndex(pieceIndex));
                REQUIRE(piece.getSide() == side);
                REQUIRE(!piece.isNone());
            }
        }
    }
}
