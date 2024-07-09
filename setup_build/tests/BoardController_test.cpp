#include <catch2/catch_all.hpp>

#include "../src/engineCore/ChessDefinitions.h"
#include "../src/engineCore/chessComponents/Board.h"
#include "../src/engineCore/chessComponents/Piece.h"
#include "../src/engineCore/chessLogic/BoardController.h"

TEST_CASE("one move")
{
    BoardController controller;

    int from_pos = 8;
    Piece piece = controller.board[from_pos];
    Move move(from_pos, 16);

    // in order for this test to work, check that target square is empty
    REQUIRE(controller.board[move.to].isNone());

    controller.doMove(move);
    REQUIRE(controller.board[move.from].isNone());
    REQUIRE(controller.board[move.to] == piece);

    controller.undoMove(move);
    REQUIRE(controller.board[move.from] == piece);
    REQUIRE(controller.board[move.to].isNone());
}

TEST_CASE("white castle")
{
    Board board;
    board[4] = Piece(King | White);
    board[7] = Piece(Rook | White);
    board[0] = Piece(Rook | White);

    BoardController controller(board);

    SECTION("king side")
    {
        Move kingside_castle(4, 6, None, false, true);
        controller.doMove(kingside_castle);

        REQUIRE(controller.board[4].isNone());
        REQUIRE(controller.board[5] == Piece(Rook | White));
        REQUIRE(controller.board[6] == Piece(King | White));
        REQUIRE(controller.board[7].isNone());

        controller.undoMove(kingside_castle);
        REQUIRE(controller.board[4] == Piece(King | White));
        REQUIRE(controller.board[5].isNone());
        REQUIRE(controller.board[6].isNone());
        REQUIRE(controller.board[7] == Piece(Rook | White));
    }
    SECTION("queen side")
    {
        Move queenside_castle(4, 2, None, false, false, true);
        controller.doMove(queenside_castle);

        REQUIRE(controller.board[4].isNone());
        REQUIRE(controller.board[3] == Piece(Rook | White));
        REQUIRE(controller.board[2] == Piece(King | White));
        REQUIRE(controller.board[0].isNone());

        controller.undoMove(queenside_castle);
        REQUIRE(controller.board[4] == Piece(King | White));
        REQUIRE(controller.board[3].isNone());
        REQUIRE(controller.board[2].isNone());
        REQUIRE(controller.board[0] == Piece(Rook | White));
    }
}

TEST_CASE("promotion")
{
    Board board;
    Piece pawn = Piece(Pawn | White);
    board[55] = pawn;

    BoardController controller(board);

    SECTION("only promotion")
    {
        Move promotion(55, 63, Queen);
        controller.doMove(promotion);

        REQUIRE(controller.board[55].isNone());
        REQUIRE(controller.board[63] == Piece(Queen | White));

        controller.undoMove(promotion, None, pawn);
        REQUIRE(controller.board[55] == Piece(Pawn | White));
        REQUIRE(controller.board[63].isNone());
    }
    SECTION("take-promotion")
    {
        board[62] = Piece(Queen | Black);
        Move take_promotion(55, 62, Queen, true);
        controller.doMove(take_promotion);

        REQUIRE(controller.board[55].isNone());
        REQUIRE(controller.board[62] == Piece(Queen | White));

        controller.undoMove(take_promotion, Queen | Black, pawn);
        REQUIRE(controller.board[55] == Piece(Pawn | White));
        REQUIRE(controller.board[62] == Piece(Queen | Black));
    }
}

TEST_CASE("Comparision of starting position")
{
    BoardController c1, c2;

    REQUIRE(c1.board == c2.board);
}

TEST_CASE("Comparision of starting position after move")
{
    BoardController c1, c2;
    Move move(52, 53);

    c1.doMove(move);
    c2.doMove(move);

    REQUIRE(c1.board == c2.board);

    c1.undoMove(move);
    c2.undoMove(move);

    REQUIRE(c1.board == c2.board);
}

TEST_CASE("after different moves")
{
    BoardController c1, c2;

    c1.doMove(Move(52, 36));
    c2.doMove(Move(52, 37));

    REQUIRE(c1.board != c2.board);
}
