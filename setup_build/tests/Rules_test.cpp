#include <catch2/catch_all.hpp>

#include <set>


#include "../src/engineCore/ChessDefinitions.h"
#include "../src/engineCore/chessComponents/Board.h"
#include "../src/engineCore/chessComponents/Piece.h"

#include "../src/engineCore/chessLogic/BoardController.h"
#include "../src/engineCore/chessLogic/Rules.h"

// basic test
TEST_CASE("possible moves for empty square") {
    Board board;
    Rules rules(board);
    vector<Move> moves;
    rules.getPossibleMoves(moves, 0);
    REQUIRE(moves.size() == 0);
}

TEST_CASE("Possible moves for a pawn") {
    Board board;
    Rules rules(board);

    SECTION("starting position") {
        board[8] = Piece(Pawn | White);
        board[48] = Piece(Pawn | Black);


        vector<Move> possibleMoves;
        rules.getPossibleMoves(possibleMoves, 8);
        REQUIRE(possibleMoves.size() == 2);
        REQUIRE(set<Move>(possibleMoves.begin(), possibleMoves.end()) ==
                set<Move>({Move(8, 16), Move(8, 24)}));

        possibleMoves.clear();
        rules.getPossibleMoves(possibleMoves, 48);
        REQUIRE(possibleMoves.size() == 2);
        REQUIRE(set<Move>(possibleMoves.begin(), possibleMoves.end()) ==
                set<Move>({Move(48, 40), Move(48, 32)}));
    }

    SECTION("mid position") {
        board[16] = Piece(Pawn | White);
        board[40] = Piece(Pawn | Black);

        vector<Move> possibleMoves;
        rules.getPossibleMoves(possibleMoves, 16);
        REQUIRE(possibleMoves.size() == 1);
        REQUIRE(set<Move>(possibleMoves.begin(), possibleMoves.end()) ==
                set<Move>({Move(16, 24)}));

        possibleMoves.clear();
        rules.getPossibleMoves(possibleMoves, 40);
        REQUIRE(possibleMoves.size() == 1);
        REQUIRE(set<Move>(possibleMoves.begin(), possibleMoves.end()) ==
                set<Move>({Move(40, 32)}));
    }

    SECTION("end position") {
        board[48] = Piece(Pawn | White);
        board[8] = Piece(Pawn | Black);

        vector<Move> possibleMoves;
        rules.getPossibleMoves(possibleMoves, 48);
        REQUIRE(possibleMoves.size() == 4);

        REQUIRE(set<Move>(possibleMoves.begin(), possibleMoves.end()) ==
                set<Move>({Move(48, 56, Queen), Move(48, 56, Rook), Move(48, 56, Bishop), Move(48, 56, Knight)}));

        possibleMoves.clear();
        rules.getPossibleMoves(possibleMoves, 8);
        REQUIRE(possibleMoves.size() == 4);
        REQUIRE(set<Move>(possibleMoves.begin(), possibleMoves.end()) ==
                set<Move>({Move(8, 0, Queen), Move(8, 0, Rook), Move(8, 0, Bishop), Move(8, 0, Knight)}));
    }
}

TEST_CASE("possible moves for king") {
    Board board;
    Rules rules(board);
    vector<Move> moves;

    SECTION("in corner") {
        board[0] = Piece(King | White);
        rules.getPossibleMoves(moves, 0);
        REQUIRE(moves.size() == 3);
        REQUIRE(set<Move>(moves.begin(), moves.end()) ==
                set<Move>({Move(0, 1), Move(0, 8), Move(0, 9)}));
    }

    SECTION("starting") {
        board[4] = Piece(King | White);
        rules.getPossibleMoves(moves, 4);
        REQUIRE(moves.size() == 5);
        REQUIRE(set<Move>(moves.begin(), moves.end()) ==
                set<Move>({Move(4, 3), Move(4, 5), Move(4, 11), Move(4, 12), Move(4, 13)}));
    }

    SECTION("in the middle") {
        board[27] = Piece(King | White);
        rules.getPossibleMoves(moves, 27);
        REQUIRE(moves.size() == 8);
        REQUIRE(set<Move>(moves.begin(), moves.end()) ==
                set<Move>({Move(27, 18), Move(27, 19), Move(27, 20), Move(27, 26), Move(27, 28), Move(27, 34), Move(27, 35), Move(27, 36)}));
    }
}

TEST_CASE("Possible moves for queen") {
    Board board;
    Rules rules(board);
    vector<Move> moves;

    SECTION("in corner") {
        board[0] = Piece(Queen | White);
        rules.getPossibleMoves(moves, 0);
        REQUIRE(moves.size() == 21);
        REQUIRE(set<Move>(moves.begin(), moves.end()) ==
                set<Move>({
                    Move(0, 1), Move(0, 2), Move(0, 3), Move(0, 4), Move(0, 5), Move(0, 6), Move(0, 7),
                    Move(0, 8), Move(0, 16), Move(0, 24), Move(0, 32), Move(0, 40), Move(0, 48), Move(0, 56),
                    Move(0, 9), Move(0, 18), Move(0, 27), Move(0, 36), Move(0, 45), Move(0, 54), Move(0, 63)
                }));
    }

    SECTION("starting") {
        board[3] = Piece(Queen | White);
        rules.getPossibleMoves(moves, 3);
        REQUIRE(moves.size() == 21);
        REQUIRE(set<Move>(moves.begin(), moves.end()) ==
                set<Move>({
                    Move(3, 0), Move(3, 1), Move(3, 2), Move(3, 4), Move(3, 5), Move(3, 6), Move(3, 7),
                    Move(3, 11), Move(3, 19), Move(3, 27), Move(3, 35), Move(3, 43), Move(3, 51), Move(3, 59),
                    Move(3, 10), Move(3, 17), Move(3, 24),
                    Move(3, 12), Move(3, 21), Move(3, 30), Move(3, 39)
                }));
    }

    SECTION("middle") {
        board[27] = Piece(Queen | White);
        rules.getPossibleMoves(moves, 27);
        REQUIRE(moves.size() == 27);
        REQUIRE(set<Move>(moves.begin(), moves.end()) ==
                set<Move>({
                    Move(27, 3), Move(27, 11), Move(27, 19), Move(27, 35), Move(27, 43), Move(27, 51), Move(27, 59),
                    Move(27, 24), Move(27, 25), Move(27, 26), Move(27, 28), Move(27, 29), Move(27, 30), Move(27, 31),
                    Move(27, 0), Move(27, 9), Move(27, 18), Move(27, 36), Move(27, 45), Move(27, 54), Move(27, 63),
                    Move(27, 6), Move(27, 13), Move(27, 20), Move(27, 34), Move(27, 41), Move(27, 48)
                }));
    }
}

TEST_CASE("possible moves for rook") {
    Board board;
    Rules rules(board);
    vector<Move> moves;

    SECTION("in corner") {
        board[0] = Piece(Rook | White);
        rules.getPossibleMoves(moves, 0);
        REQUIRE(moves.size() == 14);
        REQUIRE(set<Move>(moves.begin(), moves.end()) ==
                set<Move>({
                    Move(0, 1), Move(0, 2), Move(0, 3), Move(0, 4), Move(0, 5), Move(0, 6), Move(0, 7),
                    Move(0, 8), Move(0, 16), Move(0, 24), Move(0, 32), Move(0, 40), Move(0, 48), Move(0, 56)
                }));
    }

    SECTION("starting") {
        board[3] = Piece(Rook | White);
        rules.getPossibleMoves(moves, 3);
        REQUIRE(moves.size() == 14);
        REQUIRE(set<Move>(moves.begin(), moves.end()) ==
                set<Move>({
                    Move(3, 0), Move(3, 1), Move(3, 2), Move(3, 4), Move(3, 5), Move(3, 6), Move(3, 7),
                    Move(3, 11), Move(3, 19), Move(3, 27), Move(3, 35), Move(3, 43), Move(3, 51), Move(3, 59),
                }));
    }

    SECTION("middle") {
        board[27] = Piece(Rook | White);
        rules.getPossibleMoves(moves, 27);
        REQUIRE(moves.size() == 14);
        REQUIRE(set<Move>(moves.begin(), moves.end()) ==
                set<Move>({
                    Move(27, 3), Move(27, 11), Move(27, 19), Move(27, 35), Move(27, 43), Move(27, 51), Move(27, 59),
                    Move(27, 24), Move(27, 25), Move(27, 26), Move(27, 28), Move(27, 29), Move(27, 30), Move(27, 31),
                }));
    }
}

TEST_CASE("possible moves for bishop") {
    Board board;
    Rules rules(board);
    vector<Move> moves;

    SECTION("in corner") {
        board[0] = Piece(Bishop | White);
        rules.getPossibleMoves(moves, 0);
        REQUIRE(moves.size() == 7);
        REQUIRE(set<Move>(moves.begin(), moves.end()) ==
                set<Move>({
                    Move(0, 9), Move(0, 18), Move(0, 27), Move(0, 36), Move(0, 45), Move(0, 54), Move(0, 63)
                }));
    }

    SECTION("starting") {
        board[3] = Piece(Bishop | White);
        rules.getPossibleMoves(moves, 3);
        REQUIRE(moves.size() == 7);
        REQUIRE(set<Move>(moves.begin(), moves.end()) ==
                set<Move>({
                    Move(3, 10), Move(3, 17), Move(3, 24),
                    Move(3, 12), Move(3, 21), Move(3, 30), Move(3, 39)
                }));
    }

    SECTION("middle") {
        board[27] = Piece(Bishop | White);
        rules.getPossibleMoves(moves, 27);
        REQUIRE(moves.size() == 13);
        REQUIRE(set<Move>(moves.begin(), moves.end()) ==
                set<Move>({
                    Move(27, 6), Move(27, 13), Move(27, 20), Move(27, 34), Move(27, 41), Move(27, 48),
                    Move(27, 0), Move(27, 9), Move(27, 18), Move(27, 36), Move(27, 45), Move(27, 54), Move(27, 63)
                }));
    }
}

TEST_CASE("Possible moves for knight") {
    
}

// blocked by own
/*TEST_CASE() {
       SECTION("With blocking pieces") {

        for ( side_t side : {White, Black}) {
            SECTION("starting position") {
                board[8] = Piece(Pawn | White);
                board[48] = Piece(Pawn | Black);

                // blocking
                board[24] = Piece(Pawn | side);
                board[32] = Piece(Pawn | side);

                vector<Move> possibleMoves;
                rules.getPossibleMoves(possibleMoves, 8);
                REQUIRE(possibleMoves.size() == 1);
                REQUIRE(set<Move>(possibleMoves.begin(), possibleMoves.end()) ==
                        set<Move>({Move(8, 16)}));

                possibleMoves.clear();
                rules.getPossibleMoves(possibleMoves, 48);
                REQUIRE(possibleMoves.size() == 1);
                REQUIRE(set<Move>(possibleMoves.begin(), possibleMoves.end()) ==
                        set<Move>({Move(48, 40)}));
            }

            SECTION("mid position") {
                board[8] = Piece(Pawn | White);
                board[48] = Piece(Pawn | Black);

                //blocking
                board[16] = Piece(Pawn | side);
                board[40] = Piece(Pawn | side);

                vector<Move> possibleMoves;
                rules.getPossibleMoves(possibleMoves, 8);
                REQUIRE(possibleMoves.size() == 0);

                possibleMoves.clear();
                rules.getPossibleMoves(possibleMoves, 48);
                REQUIRE(possibleMoves.size() == 0);
            }

            SECTION("end position") {
                board[48] = Piece(Pawn | White);
                board[8] = Piece(Pawn | Black);

                //blocking
                board[56] = Piece(Pawn | side);
                board[0] = Piece(Pawn | side);

                vector<Move> possibleMoves;
                rules.getPossibleMoves(possibleMoves, 48);
                REQUIRE(possibleMoves.size() == 0);

                possibleMoves.clear();
                rules.getPossibleMoves(possibleMoves, 8);
                REQUIRE(possibleMoves.size() == 0);
            }
        }

    }
}

// take
TEST_CASE() {
        SECTION("With capturing moves") {
        board[16] = Piece(Pawn | White);
        board[25] = Piece(Pawn | Black);

        vector<Move> possibleMoves;
        rules.getPossibleMoves(possibleMoves, 16);
        REQUIRE(possibleMoves.size() == 2);
        REQUIRE(set<Move>(possibleMoves.begin(), possibleMoves.end()) ==
                set<Move>({Move(16, 24), Move(16, 25, None, true)}));

        possibleMoves.clear();
        rules.getPossibleMoves(possibleMoves, 25);
        REQUIRE(possibleMoves.size() == 2);
        REQUIRE(set<Move>(possibleMoves.begin(), possibleMoves.end()) ==
                set<Move>({Move(25, 17), Move(25, 16, None, true)}));
    }
}
*/
