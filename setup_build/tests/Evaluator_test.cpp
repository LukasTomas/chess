#include <catch2/catch_all.hpp>

#include "../src/engineCore/ChessDefinitions.h"
#include "../src/engineCore/chessComponents/Board.h"
#include "../src/engineCore/chessComponents/Piece.h"
#include "../src/engineCore/chessLogic/BoardController.h"
#include "../src/evaluation/Evaluator.h"

TEST_CASE("test one piece score")
{
    Board board;
    Evaluator evaluator(board);

    // empty board score is 0
    REQUIRE(evaluator.CountMaterial(White) == 0);
    REQUIRE(evaluator.CountMaterial(Black) == 0);

    SECTION("one pawn score")
    {
        board[0] = Piece(Pawn | White);
        board[63] = Piece(Pawn | Black);

        REQUIRE(evaluator.CountMaterial(White) == Evaluator::PAWN_VALUE);
        REQUIRE(evaluator.CountMaterial(Black) == Evaluator::PAWN_VALUE);
        REQUIRE(evaluator.CountMaterial(White) == evaluator.CountMaterial(Black));
    }
    SECTION("one knight score")
    {
        board[0] = Piece(Knight | White);
        board[63] = Piece(Knight | Black);

        REQUIRE(evaluator.CountMaterial(White) == Evaluator::KNIGHT_VALUE);
        REQUIRE(evaluator.CountMaterial(Black) == Evaluator::KNIGHT_VALUE);
        REQUIRE(evaluator.CountMaterial(White) == evaluator.CountMaterial(Black));
    }
    SECTION("one bishop score")
    {
        board[0] = Piece(Bishop | White);
        board[63] = Piece(Bishop | Black);

        REQUIRE(evaluator.CountMaterial(White) == Evaluator::BISHOP_VALUE);
        REQUIRE(evaluator.CountMaterial(Black) == Evaluator::BISHOP_VALUE);
        REQUIRE(evaluator.CountMaterial(White) == evaluator.CountMaterial(Black));
    }
    SECTION("one rook score")
    {
        board[0] = Piece(Rook | White);
        board[63] = Piece(Rook | Black);

        REQUIRE(evaluator.CountMaterial(White) == Evaluator::ROOK_VALUE);
        REQUIRE(evaluator.CountMaterial(Black) == Evaluator::ROOK_VALUE);
        REQUIRE(evaluator.CountMaterial(White) == evaluator.CountMaterial(Black));
    }
    SECTION("one queen score")
    {
        board[0] = Piece(Queen | White);
        board[63] = Piece(Queen | Black);

        REQUIRE(evaluator.CountMaterial(White) == Evaluator::QUEEN_VALUE);
        REQUIRE(evaluator.CountMaterial(Black) == Evaluator::QUEEN_VALUE);
        REQUIRE(evaluator.CountMaterial(White) == evaluator.CountMaterial(Black));
    }
    SECTION("king score")
    {
        board[0] = Piece(King | White);
        board[63] = Piece(King | Black);

        REQUIRE(evaluator.CountMaterial(White) == 0);
        REQUIRE(evaluator.CountMaterial(Black) == 0);
    }
}

TEST_CASE("one side one piece")
{
    Board board;
    Evaluator evaluator(board);

    SECTION("one pawn score")
    {
        board[0] = Piece(Pawn | White);

        REQUIRE(evaluator.CountMaterial(White) == Evaluator::PAWN_VALUE);
        REQUIRE(evaluator.CountMaterial(Black) == 0);
    }
    SECTION("one knight score")
    {
        board[0] = Piece(Knight | White);

        REQUIRE(evaluator.CountMaterial(White) == Evaluator::KNIGHT_VALUE);
        REQUIRE(evaluator.CountMaterial(Black) == 0);
    }
    SECTION("one bishop score")
    {
        board[0] = Piece(Bishop | White);

        REQUIRE(evaluator.CountMaterial(White) == Evaluator::BISHOP_VALUE);
        REQUIRE(evaluator.CountMaterial(Black) == 0);
    }
    SECTION("one rook score")
    {
        board[0] = Piece(Rook | White);

        REQUIRE(evaluator.CountMaterial(White) == Evaluator::ROOK_VALUE);
        REQUIRE(evaluator.CountMaterial(Black) == 0);
    }
    SECTION("one queen score")
    {
        board[0] = Piece(Queen | White);

        REQUIRE(evaluator.CountMaterial(White) == Evaluator::QUEEN_VALUE);
        REQUIRE(evaluator.CountMaterial(Black) == 0);
    }
    SECTION("king score")
    {
        board[0] = Piece(King | White);

        REQUIRE(evaluator.CountMaterial(White) == 0);
        REQUIRE(evaluator.CountMaterial(Black) == 0);
    }
}

TEST_CASE("test all pieces")
{
    for (side_t side : {White, Black})
    {
        Board board;
        Evaluator evaluator(board);

        board[0] = Piece(Pawn | side);
        board[1] = Piece(Knight | side);
        board[2] = Piece(Bishop | side);
        board[3] = Piece(Rook | side);
        board[4] = Piece(Queen | side);
        board[5] = Piece(King | side);

        int sum = Evaluator::PAWN_VALUE + Evaluator::KNIGHT_VALUE + Evaluator::BISHOP_VALUE + Evaluator::ROOK_VALUE + Evaluator::QUEEN_VALUE;
        REQUIRE(evaluator.CountMaterial(side) == sum);
    }
}

TEST_CASE("multiple pieces")
{
    const int count = 4;
    for (side_t side : {White, Black})
    {
        Board board;
        Evaluator evaluator(board);

        for (int i = 0; i < count; i++)
        {
            board[i] = Piece(Pawn | side);
        }

        int sum = count * Evaluator::PAWN_VALUE;
        REQUIRE(evaluator.CountMaterial(side) == sum);
    }
}

TEST_CASE("sides have different number of pieces")
{
    Board board;
    Evaluator evaluator(board);

    board[0] = Piece(Pawn | White);
    board[1] = Piece(Knight | White);
    board[2] = Piece(Bishop | White);
    board[3] = Piece(Rook | White);
    board[4] = Piece(Queen | White);
    board[5] = Piece(King | White);

    board[6] = Piece(Pawn | Black);
    board[7] = Piece(Knight | Black);
    board[8] = Piece(Bishop | Black);
    board[10] = Piece(Queen | Black);

    int whiteSum = Evaluator::PAWN_VALUE + Evaluator::KNIGHT_VALUE + Evaluator::BISHOP_VALUE + Evaluator::ROOK_VALUE + Evaluator::QUEEN_VALUE;
    int blackSum = Evaluator::PAWN_VALUE + Evaluator::KNIGHT_VALUE + Evaluator::BISHOP_VALUE                         + Evaluator::QUEEN_VALUE;

    REQUIRE(evaluator.CountMaterial(White) == whiteSum);
    REQUIRE(evaluator.CountMaterial(Black) == blackSum);
    REQUIRE(evaluator.CountMaterial(White) != evaluator.CountMaterial(Black));
}
