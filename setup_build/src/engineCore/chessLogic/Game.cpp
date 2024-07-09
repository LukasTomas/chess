#include "Game.h"

#include <stdexcept>
#include <iostream>
#include <fstream>

const array<int, MAX_ARRAY_SIZE> Game::POINTS = {0, 1, Game::MINOR_PIECE_POINTS, Game::MINOR_PIECE_POINTS, 5, 9};


Game::Game( bool threeMoveRulePermited, bool fiftyMoveRulePermited, const string & filename )
    : rules(Rules(board)), threeMoveRulePermited(threeMoveRulePermited), fiftyMoveRulePermited(fiftyMoveRulePermited),
      boardController(), board(boardController.board) {

    // set information about chess game
    gameInfo.moveNumber = 1;
    gameInfo.onTurn = White;
    gameInfo.fiftyMovesCount = 0;


    setPieceInfoFromBoard();

    if ( filename != "" ) {
        ifstream i(filename);

        Move m;
        int j = 0;
        while ( i >> m ) {
            if ( j > 9 )
                break;

            doMove(m);

            j++;
        }

        i.close();
    }
}

Game::Game( Board & board, bool threeMoveRulePermited, bool fiftyMoveRulePermited )
    : rules(Rules(board)), threeMoveRulePermited(threeMoveRulePermited), fiftyMoveRulePermited(fiftyMoveRulePermited),
      boardController(board), board(boardController.board) {

    gameInfo.moveNumber = 1;
    gameInfo.onTurn = White;
    gameInfo.fiftyMovesCount = 0;

    setPieceInfoFromBoard();
}



void Game::_setDrawPosition1() {
    board[0] = PieceType::King | White;
    board[60] = PieceType::King | Black;
}

void Game::_setDrawPosition21() {
    board[0] = PieceType::King | White;
    board[1] = PieceType::Knight | White;

    board[60] = PieceType::King | Black;
}

void Game::_setDrawPosition22() {
    board[0] = PieceType::King | White;
    board[2] = PieceType::Bishop | White;

    board[60] = PieceType::King | Black;
}

void Game::_setDrawPosition3() {
    board[0] = PieceType::King | White;
    board[2] = PieceType::Bishop | White;

    board[60] = PieceType::King | Black;
    board[59] = PieceType::Bishop | Black;
}

void Game::_setDrawPosition4() {
    board[0] = PieceType::King | White;
    board[5] = PieceType::Rook | White;

    board[60] = PieceType::King | Black;
}

void Game::_setDrawPosition5() {
    board[0] = PieceType::King | White;
    board[5] = PieceType::Pawn | White;

    board[60] = PieceType::King | Black;
}


void Game::setPieceInfoFromBoard() {
    for ( size_t square = 0; square < board.n_squares; square++ ) {
        Piece pieceOnSquare = board[square];

        piece_t piece = pieceOnSquare.getPieceType();
        uint8_t index = pieceOnSquare.toIndex();

        if ( piece == PieceType::None )    // no piece here
            continue;

        PieceInfo & pieceInfo = pieceOnSquare.isWhite() ? whitePieces[index] : blackPieces[index];

        pieceInfo.positions.insert(square);
    }
}

void Game::updateGameInfo( const Move & move ) {
    gameInfo.moves.push_back(move);
    gameInfo.moveNumber++;
    gameInfo.onTurn = gameInfo.onTurn == White ? Black : White;
}

void Game::deupdateGameInfo() {
    gameInfo.moves.pop_back();
    gameInfo.moveNumber--;
    gameInfo.onTurn = gameInfo.onTurn == White ? Black : White;
}

void Game::pieceInfoChangePosition( Piece piece, uint8_t from, uint8_t to ) {
    uint8_t pieceIndex = piece.toIndex();
    PieceInfo & pieceInfo = piece.isWhite() ? whitePieces[pieceIndex] : blackPieces[pieceIndex];

    //cout << "before erase |" << PieceType::getName(PieceType::pieceNumber(piece)) << "|"  << endl;

    pieceInfo.positions.erase(from);
    //cout << "after erase" << endl;
    pieceInfo.positions.insert(to);
}

void Game::pieceInfoChangePositionPromotion( Piece startingPiece, Piece promotionPiece, uint8_t from, uint8_t to ) {
    uint8_t startingPieceIndex = startingPiece.toIndex();
    PieceInfo & pieceInfo = startingPiece.isWhite() ? whitePieces[startingPieceIndex] : blackPieces[startingPieceIndex];
    pieceInfo.positions.erase(from);

    int promotionPieceIndex = promotionPiece.toIndex();
    PieceInfo & promotingPieceInfo = startingPiece.isWhite() ? whitePieces[promotionPieceIndex] : blackPieces[promotionPieceIndex];
    promotingPieceInfo.positions.insert(to);
}

bool Game::checkThreeMoveRule() {
    if ( !threeMoveRulePermited )
        return false;

    if ( previousBoards.size() <= 8 )
        return false;

    int threeMoveRuleCount = 0;
    Board & threeMoveRuleBoard = previousBoards.back();
    for ( size_t i = 1; i <= 4; i++ ) {

        Board & b = previousBoards[previousBoards.size() - 1 - 2*i];
        if ( b == threeMoveRuleBoard )
            threeMoveRuleCount += 1;
    }

    return threeMoveRuleCount >= 2;
}

bool Game::checkCastlePossible( const Move & move ) {
    MovedPieces & playing = gameInfo.onTurn == White ? gameInfo.whiteMovedPieces : gameInfo.blackMovedPieces;
    if ( playing.kingMoved || (playing.leftRookMoved && move.isQueenCastle) || (playing.rightRookMoved && move.isKingCastle) )
        return false;       // cannot make castle, king moved;

    // check if squares empty and no possible check
    int squaresBetween = abs(move.from - move.to);

    uint8_t currentSquare = move.from;

    // check if now is check
    if ( playingSideHasCheck() )
        return false;

    currentSquare += move.from < move.to ? 1 : -1;

    //check if no possible check in a way
    for ( int i = 0; i < squaresBetween; i++ ) {
        if ( isAtacking((gameInfo.onTurn == White) ? Black : White, currentSquare) )
            return false;

        currentSquare += move.from < move.to ? 1 : -1;
    }


    return true;
}

// DO & UNDO MOVE ----------------------------------------------------------------------------------------
bool Game::doMove( const Move & move ) {
    Cache cache = {0};

    Piece startingSquare = board[move.from];
    Piece targetSquare = board[move.to];

    piece_t startingPiece = startingSquare.getPieceType();
    piece_t targetPiece = targetSquare.getPieceType();

    // check and update 50 move rule
    cache.fiftyMoveCount = gameInfo.fiftyMovesCount;
    if ( startingPiece != PieceType::Pawn && !move.isTake )
        gameInfo.fiftyMovesCount += 1;
    else
        gameInfo.fiftyMovesCount = 0;

    if ( gameInfo.fiftyMovesCount > 100 && fiftyMoveRulePermited ) {
        // cout << "FIFTY MOVES RULE" << endl;
        return false;
    }


    if ( startingPiece == PieceType::None )
        throw invalid_argument("Cannot move with 'NONE' piece (shrugging)");

    if ( startingSquare.getSide() != gameInfo.onTurn )
        throw invalid_argument("This side is not on turn");

    bool isTake = /*targetPiece != PieceType::None*/ move.isTake;
    if ( isTake && startingSquare.sameSide(targetSquare) )
        throw invalid_argument("Cannot take piece of the same side");

    MovedPieces & playing = gameInfo.onTurn == White ? gameInfo.whiteMovedPieces : gameInfo.blackMovedPieces;
    if ( move.isCastle() ) {
        if ( !checkCastlePossible(move) )
            return false;
        else
            playing.kingMoved = true;   // no need to set rooks
    } else if ( startingPiece == PieceType::King && !playing.kingMoved ) {
        playing.kingMoved = true;
        cache.pieceMovedSet.kingMoved = true;
    } else if ( startingPiece == PieceType::Rook ) {
        uint8_t rightRookPos = gameInfo.onTurn == White ? 7 : 63;
        uint8_t leftRookPos = gameInfo.onTurn == White ? 0 : 56;

        if ( move.from == rightRookPos && !playing.rightRookMoved ) {
            playing.rightRookMoved = true;
            cache.pieceMovedSet.rightRookMoved = true;
        } else if ( move.from == leftRookPos && !playing.leftRookMoved ) {
            playing.leftRookMoved = true;
            cache.pieceMovedSet.leftRookMoved = true;
        }
    }

    if ( isTake ) {
        uint8_t targetPieceIndex = targetSquare.toIndex();
        PieceInfo & pieceInfo = targetSquare.isWhite() ? whitePieces[targetPieceIndex] : blackPieces[targetPieceIndex];

        size_t erased = pieceInfo.positions.erase(move.to);
        if ( erased == 0 )
            throw logic_error("no position erased");

        cache.wasTake = true;
    }

    // save previous board state for 3 move rule checking
    // TODO save just 3 last
    previousBoards.push_back(board);

    // update board
    boardController.doMove(move);

    if ( move.isPromotion() )
        cache.beforePromotionPiece = startingSquare;

    int catleRelativeRookStart = move.isKingCastle ? +1 : -2;
    int castleRelativeRookEnd = move.isKingCastle ? +1 : -1;

    // update moving piece information about positions
    if ( move.isPromotion() )
        pieceInfoChangePositionPromotion(startingSquare, Piece(move.promotion | gameInfo.onTurn), move.from, move.to);
    else if ( move.isCastle() ) {
        pieceInfoChangePosition(Piece(PieceType::King | gameInfo.onTurn), move.from, move.to);    // king moved
        pieceInfoChangePosition(Piece(PieceType::Rook | gameInfo.onTurn), move.to + catleRelativeRookStart, move.from + castleRelativeRookEnd);    // rook moved but other way
    } else
        pieceInfoChangePosition(startingSquare, move.from, move.to);

    side_t sidePlaying = gameInfo.onTurn;       // before updating gameInfo, because sideOnTurn will be changed
    updateGameInfo(move);

    cache.targetPiece = targetSquare;

    /* cannot check on start of this methods
        if playing side has check now, its next move can place some piece in the way
        playing side can make some move, which can place its king in the check
    */
   caches.push(cache);

    bool isThreeMoveRule = checkThreeMoveRule();
    if ( isThreeMoveRule )
        cout << "THREE MOVE RULE" << endl;


    bool hasCheck = isCheck(sidePlaying);
    if ( hasCheck || isThreeMoveRule ) {
        undoMove();     // pops back cache
        return false;
    }


        array<PieceInfo, MAX_ARRAY_SIZE> whitePieces2 = whitePieces;
    array<PieceInfo, MAX_ARRAY_SIZE> blackPieces2 = blackPieces;
    setPieceInfoFromBoard();
    if ( whitePieces2 != whitePieces || blackPieces2 != blackPieces )
        throw logic_error("wront update of positions");

    return true;
}

void Game::undoMove() {
    Cache & cache = caches.top();

    if ( gameInfo.moves.empty() ) {
        cout << "Cannot undo move: no move was made" << endl;
        return;     // no move was made
    }

    Move & lastMove = gameInfo.moves.back();
    Piece startingSquare = board[lastMove.to];

    gameInfo.fiftyMovesCount = cache.fiftyMoveCount;

    deupdateGameInfo();

    // update board
    boardController.undoMove(lastMove, cache.targetPiece, cache.beforePromotionPiece);

    int catleRelativeRookStart = lastMove.isKingCastle ? +1 : -2;
    int castleRelativeRookEnd = lastMove.isKingCastle ? +1 : -1;

    // update moving piece information about positions
    if ( lastMove.isPromotion() )
        pieceInfoChangePositionPromotion(Piece(lastMove.promotion | gameInfo.onTurn), cache.beforePromotionPiece, lastMove.to, lastMove.from);
    else if ( lastMove.isCastle() ) {
        pieceInfoChangePosition(Piece(PieceType::King | gameInfo.onTurn), lastMove.to, lastMove.from);    // king moved
        pieceInfoChangePosition(Piece(PieceType::Rook | gameInfo.onTurn), lastMove.from + castleRelativeRookEnd, lastMove.to + catleRelativeRookStart);    // rook moved but other way
    } else
        pieceInfoChangePosition(startingSquare, lastMove.to, lastMove.from);

    if ( cache.wasTake ) {
        uint8_t tookPieceIndex = cache.targetPiece.toIndex();
        PieceInfo & pieceInfo = cache.targetPiece.isWhite() ? whitePieces[tookPieceIndex] : blackPieces[tookPieceIndex];

        pieceInfo.positions.insert(lastMove.to);
    }

    MovedPieces & playing = gameInfo.onTurn == White ? gameInfo.whiteMovedPieces : gameInfo.blackMovedPieces;
    if ( lastMove.isCastle() )
        playing.kingMoved = false;
    else if ( cache.pieceMovedSet.kingMoved )
        playing.kingMoved = false;
    else if ( cache.pieceMovedSet.leftRookMoved )
        playing.leftRookMoved = false;
    else if ( cache.pieceMovedSet.rightRookMoved )
        playing.rightRookMoved = false;


    array<PieceInfo, MAX_ARRAY_SIZE> whitePieces2 = whitePieces;
    array<PieceInfo, MAX_ARRAY_SIZE> blackPieces2 = blackPieces;
    setPieceInfoFromBoard();
    if ( whitePieces2 != whitePieces || blackPieces2 != blackPieces )
        throw logic_error("wront deupdate of positions");

    caches.pop();
    previousBoards.pop_back();
}

// CHECK --------------------------------------------------------------------------------------------------
bool Game::playingSideHasCheck() const {
    return isCheck(gameInfo.onTurn);
}

bool Game::otherSideHasCheck() const {
    side_t otherSide = gameInfo.onTurn == White ? Black : White;
    return isCheck(otherSide);
};

bool Game::isCheck( side_t side ) const {
    int kingIndex = pieceToIndex(PieceType::King);
    const PieceInfo & kingPieceInfo = (side == White) ? whitePieces[kingIndex] : blackPieces[kingIndex];
    if ( kingPieceInfo.positions.empty() )
        throw logic_error(SIDE_NAMES.at(side) + " has no king");

    uint8_t kingPosition = *kingPieceInfo.positions.begin();

    return isAtacking((side == White) ? Black : White, kingPosition);
}


bool Game::isAtacking( side_t side, uint8_t square ) const {
    const array<PieceInfo, MAX_ARRAY_SIZE> & enemyPieceInfoArray = (side == White) ? whitePieces : blackPieces;
    for ( int i = 0; i < MAX_ARRAY_SIZE; i++ ) {                         // for every enemy piece type
        const PieceInfo & enemyPieceInfo = enemyPieceInfoArray[i];
        piece_t currentPiece = pieceFromIndex(i);

        if ( enemyPieceInfo.positions.empty() )     // currentPiece == PieceType::King  - would be WRONG - even king can "take" other side's king
            continue;

        for ( uint8_t currentPiecePosition : enemyPieceInfo.positions ) {      // for every piece of this type
            vector<Move> possibleMoves;
            rules.getPossibleMoves(possibleMoves, currentPiecePosition);

            //for every move check if it's take of king
            for ( const Move & move : possibleMoves ) {                         // for every possible move
                if ( /*move.isTake &&*/ move.to == square ) {
                    //cout << PieceType::getSideName(side) << " has CHECK" << endl;
                    return true;
                }
            }

         }
    }

    return false;
}


bool Game::isDraw() const {
    int whitePoints = getPoints(White);
    int blackPoints = getPoints(Black);
    //cout << "white points: " << whitePoints << " | black points: " << blackPoints << endl;

    // two king
    if ( whitePoints == 0 && blackPoints == 0 )
        return true;

    // check if side has pawn
    if ( !blackPieces[ pieceToIndex(Pawn) ].positions.empty() || !whitePieces[ pieceToIndex(Pawn) ].positions.empty() )
        return false;

    // king vs king with minor piece ( bishop | knight )
    if ( (whitePoints == 0 && blackPoints == MINOR_PIECE_POINTS) || (blackPoints == 0 && whitePoints == MINOR_PIECE_POINTS) )
        return true;

    // king and bishop vs king and same-color-bishop
    if ( whitePoints == MINOR_PIECE_POINTS && blackPoints == MINOR_PIECE_POINTS ) {

        const set<uint8_t> & blackBishopPositions = blackPieces[ pieceToIndex(Bishop) ].positions;
        const set<uint8_t> & whiteBishopPositions = whitePieces[ pieceToIndex(Bishop) ].positions;

        if ( blackBishopPositions.size() == 0 || whiteBishopPositions.size() == 0 )
            return false;

        // check if bishops are on the same squares
        if ( board.getSquareColor(*blackBishopPositions.begin()) == board.getSquareColor(*whiteBishopPositions.begin()) )
            return true;
    }

    return false;
}

bool Game::isStalemate() const {
    vector<Move> moves;     // all allowed moves
    getAllAllowedMoves(moves);

    return moves.size() == 0 && !playingSideHasCheck();
}

bool Game::isCheckMate() const {
    vector<Move> moves;     // all allowed moves
    getAllAllowedMoves(moves);

    return moves.size() == 0 && playingSideHasCheck();
}

void Game::getAllAllowedMoves( vector<Move> & moves ) const {

}


void Game::saveMoves( const string & file ) const {
    ofstream o(file);

    for ( const Move & m : gameInfo.moves ) {
        m.print();
        cout << endl;

        if (m.from == 255 || m.to == 255)
            throw logic_error("invalid move");
        o << m;
    }

    o.close();
}


int Game::getPoints( side_t side ) const {
    int points = 0;

    const array<PieceInfo, MAX_ARRAY_SIZE> & pieceInfoArray = (side == White) ? whitePieces : blackPieces;
    for ( int i = 0; i < MAX_ARRAY_SIZE; i++ ) {
        int piecePoints = POINTS[i];
        points += pieceInfoArray[i].positions.size() * piecePoints;
    }

    return points;
}


// MOVES GENERATION -------------------------------------------------------------------------------
void Game::getPseudoLegalMovesFromSquare( vector<Move> & moves, uint8_t square ) const {
    rules.getPossibleMoves(moves, square);
}

void Game::getPseudoLegalMoves( vector<Move> & moves, piece_t piece ) const {
    const PieceInfo & pieceInfo = getPlayingPieceInfo(piece);
    for ( uint8_t piecePos : pieceInfo.positions )
        rules.getPossibleMoves(moves, piecePos);
}

void Game::getPseudoLegalMoves( vector<Move> & moves ) const {
    const array<PieceInfo, MAX_ARRAY_SIZE> & playing = (gameInfo.onTurn == White) ? whitePieces : blackPieces;

    for ( int i = 0; i < MAX_ARRAY_SIZE; i++ ) {                         // for every enemy piece type
        const PieceInfo & pieceInfo = playing[i];
        for ( uint8_t piecePos : pieceInfo.positions ) {
            rules.getPossibleMoves(moves, piecePos);
        }
    }
}

bool Game::getLegalMoves( vector<Move> & legalMoves, piece_t piece ) {
    if ( isDraw() )
        return true;

    vector<Move> pseudoLegalMoves;
    getPseudoLegalMoves(pseudoLegalMoves, piece);


    for ( const Move & move : pseudoLegalMoves ) {

        bool moveMade = doMove(move);
        if ( moveMade ) {
            undoMove();
            legalMoves.push_back(move);
        }
    }


    if ( legalMoves.empty() )
        return true;

    return false;
}

bool Game::getLegalMoves( vector<Move> & legalMoves ) {
    if ( isDraw() ) {
        // cout << "DRAFT" << endl;
        return true;
    }

    vector<Move> pseudoLegalMoves;
    getPseudoLegalMoves(pseudoLegalMoves);


    for ( const Move & move : pseudoLegalMoves ) {

        bool moveMade = doMove(move);
        if ( moveMade ) {
            undoMove();
            legalMoves.push_back(move);
        }
    }


    if ( legalMoves.empty() ) {
        /*cout << PieceType::getSideName(sidePlaying()) << " has no possible moves" << endl;

        if ( isStalemate() )
            cout << "STALEMATE" << endl;
        else if ( isCheckMate() )
            cout << "CHECKMATE" << endl;
        else
            throw logic_error("invalid game result state");
*/
        return true;
    }

    return false;
}
