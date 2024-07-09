#ifndef __GAME_H__
#define __GAME_H__

#include "../ChessDefinitions.h"

#include "../chessComponents/Board.h"
#include "../chessComponents/Piece.h"
#include "../chessComponents/Move.h"

#include "Rules.h"
#include "BoardController.h"

#include <vector>
#include <array>
#include <set>
#include <stack>
using namespace std;

class Game {
public:
    static const int MINOR_PIECE_POINTS = 3;
    static const array<int, MAX_ARRAY_SIZE> POINTS;

    // is castle possible ? structure to track if important pieces moved
    struct MovedPieces {
        bool kingMoved;
        bool leftRookMoved;
        bool rightRookMoved;

        bool operator==( const MovedPieces & m ) const {
            return this->kingMoved == m.kingMoved && this->leftRookMoved == m.leftRookMoved &&
             this->rightRookMoved == m.rightRookMoved;
        }
    };

    struct GameInfo {
        side_t onTurn;      // side on turn
        int moveNumber;
        vector<Move> moves;

        bool isCheck;       // now

        MovedPieces whiteMovedPieces;
        MovedPieces blackMovedPieces;

        int fiftyMovesCount;

        bool operator==( const GameInfo & m ) const {
            return this->onTurn == m.onTurn && this->moveNumber == m.moveNumber && this->moves == m.moves &&
             this->isCheck == m.isCheck && this->whiteMovedPieces == m.whiteMovedPieces &&
             this->blackMovedPieces == m.blackMovedPieces && this->fiftyMovesCount == m.fiftyMovesCount;
        }
    };

    struct PieceInfo {
        set<uint8_t> positions;

        PieceInfo & operator=( const PieceInfo & m ) {
            if (this->positions != m.positions)
                this->positions = m.positions;

            return *this;
        }

        bool operator==( const PieceInfo & m ) const {
            return this->positions == m.positions;
        }
    };


    Game( bool threeMoveRulePermited = true, bool fiftyMoveRulePermited = true, const string & filename = "" );
    Game( Board & board, bool threeMoveRulePermited = true, bool fiftyMoveRulePermited = true );

    bool doMove( const Move & move );
    void undoMove();

    const Board & getBoard() const {
        return board;
    }

    BoardController & getController() {
        return boardController;
    }

    const GameInfo & getGameInfo() const {
        return gameInfo;
    }

    const PieceInfo & getPlayingPieceInfo(piece_t piece) const {
        uint8_t pieceIndex = pieceToIndex(piece);
        return (gameInfo.onTurn == White) ? whitePieces[pieceIndex] : blackPieces[pieceIndex];
    }

    bool isSidePlaying( side_t side ) const {
        return gameInfo.onTurn == side;
    }

    side_t sidePlaying() const {
        return gameInfo.onTurn;
    }

    bool playingSideHasCheck() const;
    bool otherSideHasCheck() const;
    bool isCheck( side_t side ) const;
    bool isAtacking( side_t side, uint8_t square ) const;


    bool isDraw() const;

    bool isStalemate() const;
    bool isCheckMate() const;

    void getAllAllowedMoves( vector<Move> & moves ) const;

    void saveMoves( const string & file ) const;

    int getPoints( side_t side ) const;

    void getPseudoLegalMovesFromSquare( vector<Move> & moves, uint8_t square ) const;
    void getPseudoLegalMoves( vector<Move> & moves, piece_t piece ) const;
    void getPseudoLegalMoves( vector<Move> & moves ) const;
    bool getLegalMoves( vector<Move> & legalMoves );
    bool getLegalMoves( vector<Move> & legalMoves, piece_t piece );

    Game& operator=( const Game & game ) {
            this->caches = game.caches;
            this->previousBoards = game.previousBoards;

            this->board = game.board;
            this->boardController = BoardController(board);

            this->gameInfo = game.gameInfo;

            this->whitePieces = game.whitePieces;
            this->blackPieces = game.blackPieces;

        return *this;
    }

    bool operator==( const Game & game ) const {
        return this->board == game.board && this->gameInfo == game.gameInfo && this->caches == game.caches &&
         whitePieces == game.whitePieces && blackPieces == game.blackPieces && previousBoards == game.previousBoards;
    }

    bool operator!=( const Game & game ) const {
        return !(*this == game);
    }

private:
    struct Cache {
        bool wasTake;
        Piece targetPiece;
        Piece beforePromotionPiece;
        int fiftyMoveCount;

        MovedPieces pieceMovedSet;

        bool operator==( const Cache & m ) const {
            return this->wasTake == m.wasTake && this->targetPiece == m.targetPiece &&
             this->beforePromotionPiece == m.beforePromotionPiece && this->fiftyMoveCount == m.fiftyMoveCount &&
             this->pieceMovedSet == m.pieceMovedSet;
        }
    };
    stack<Cache> caches;

    // for 3 move rule
    vector<Board> previousBoards;


    Board & board;
    BoardController boardController;

    GameInfo gameInfo = {0};
    Cache cache = {0};
    Rules rules;

    const bool threeMoveRulePermited, fiftyMoveRulePermited;

    array<PieceInfo, MAX_ARRAY_SIZE> whitePieces, blackPieces;

    void _setDrawPosition1();
    void _setDrawPosition21();
    void _setDrawPosition22();
    void _setDrawPosition3();
    void _setDrawPosition4();
    void _setDrawPosition5();

    void setPieceInfoFromBoard();

    void updateGameInfo( const Move & move );
    void deupdateGameInfo();

    void pieceInfoChangePosition( Piece piece, uint8_t from, uint8_t to );
    void pieceInfoChangePositionPromotion( Piece startingPiece, Piece promotionPiece, uint8_t from, uint8_t to );

    bool checkThreeMoveRule();
    bool checkCastlePossible( const Move & move );

};

#endif  // __GAME_H__
