#include "MinimaxGame.h"

#include "../engineCore/ChessDefinitions.h"
#include "../terminal/Logging.h"

#include <unistd.h>
#include <ctime>
#include <algorithm>
#include <climits>

MinimaxGame::MinimaxGame( const Constraint & constraint)
: /*game(true, true, "moves.txt"),*/ constraint(constraint), rules(game.getBoard()), search(game, constraint) {
    // srand(time(nullptr));
}

bool MinimaxGame::makeMove() {
    vector<Move> moves;
    bool end = game.getLegalMoves(moves);
    if ( end || constraintSatisfied )
        return false;

    int depth = 1;

    Move & bestMove = moves[rand() % moves.size()];
    game.doMove(bestMove);
    int evaluation = -search.AlphaBeta(depth - 1, Search::NEGATIVE_INF, Search::INF);
    game.undoMove();
    int bestEvaluation = evaluation;

    for ( Move & move : moves ) {
        game.doMove(move);
        int evaluation = -search.AlphaBeta(depth - 1, Search::NEGATIVE_INF, Search::INF);
        game.undoMove();

        // cannot be '>=' because at start of the game, it will always choose the last move
        if ( evaluation > bestEvaluation ) {
            bestEvaluation = evaluation;
            bestMove = move;
        }
    }

    bool moveMade = game.doMove(bestMove);

    constraintSatisfied = search.evaluator.ConstraintSatisfied();

    if ( !moveMade )
        throw logic_error("legal move could no have been made");

   return true;
}


// if the best move has score 0.8, than it can choose move with score at least 0.7
const double INTERVAL_CRITERIUM = 0.1;

struct IndexValue {
    int index;
    double value;

    bool operator==( const IndexValue & other ) const {
        return value == other.value;
    }

    bool operator<( const IndexValue & other ) const {
        return value < other.value;
    }

    bool operator>( const IndexValue & other ) const {
        return value > other.value;
    }
};

bool MinimaxGame::makeMove_2() {
    vector<Move> moves;
    bool end = game.getLegalMoves(moves);
    if ( end || constraintSatisfied || game.getGameInfo().moveNumber > 100 )
        return false;

    int depth = 2;


    vector<IndexValue> indexValues;


    Move & bestMove = moves[rand() % moves.size()];
    game.doMove(bestMove);
    double evaluation = search.evaluator.Evaluate(game.sidePlaying());
    game.undoMove();
    double bestEvaluation = evaluation;


    for ( size_t i = 0; i < moves.size(); i++ ) {
        Move & move = moves[i];

        game.doMove(move);
        // double evaluation = search.evaluator.Evaluate(game.sidePlaying());
        double evaluation = search.minimax(depth-1);

        // bool canConstraintBeSatisfied = this->isConstraintSatisfiable();
        // if ( !canConstraintBeSatisfied ) {
        //     game.undoMove();
        //     continue;
        // }

        IndexValue indexValue = {i, evaluation};
        auto it = lower_bound(indexValues.begin(), indexValues.end(), indexValue);
        indexValues.insert(it, indexValue);

        if (search.evaluator.ConstraintSatisfied()) {
            constraintSatisfied = true;
            cout << "satisfied" << endl;
            return true;
        }

        game.undoMove();

        // cannot be '>=' because at start of the game, it will always choose the last move
        // if ( evaluation > bestEvaluation ) {
        //     bestEvaluation = evaluation;
        //     bestMove = move;
        // }
    }

    bestEvaluation = indexValues.back().value;
    // cout << "best evaluation: " << bestEvaluation << endl;

    double minChoosableEval = bestEvaluation - INTERVAL_CRITERIUM;
    int minChoosableMoveIndex = indexValues.size() - 1;
    for ( int i = indexValues.size() - 2; i >= 0; i-- ) {
        if ( indexValues[i].value < minChoosableEval )
            break;
        else
            minChoosableMoveIndex = i;
    }
    int maxChoosableMoveIndex = indexValues.size() - 1;

    int index = rand() % (maxChoosableMoveIndex - minChoosableMoveIndex + 1) + minChoosableMoveIndex;
    // cout << "choose index: " << index << " - eval: " << indexValues[index].value << endl << endl;

    bestMove = moves[indexValues[index].index];
    bool moveMade = game.doMove(bestMove);
    if ( !moveMade )
        throw logic_error("legal move could no have been made");

    constraintSatisfied = search.evaluator.ConstraintSatisfied();

   return true;
}

const Board & MinimaxGame::getBoard() const {
    return game.getBoard();
}


// PRIVATE METHODS --------------------------------------------------------------------------------------------

bool MinimaxGame::isConstraintSatisfiable() {
    // no king
    array<int, MAX_ARRAY_SIZE_NO_KING> whitePieceCounts = game.getController().getPieceCounts(Side::White);

    string errorMsg = constraint.toString() + "\n(";
    for ( int i = MAX_ARRAY_SIZE_NO_KING-1; i >= 0; i-- ) 
        errorMsg += to_string(whitePieceCounts[i]) + ", ";

    // first check pawns
    int constraintPawns = constraint.getConstrain(PieceType::Pawn);
    int currentPawns = whitePieceCounts[pieceToIndexWithoutKing(PieceType::Pawn)];
    if ( constraintPawns > currentPawns ) {
        LOG_NOTICE << "Constraint can't be satisifed, not enought pawns " << currentPawns << "|" << constraintPawns << endl;
        cout << errorMsg << endl;
        return false;
    }

    // check other pieces
    int possiblePromotions = currentPawns - constraintPawns;
    for ( piece_t piece : {Rook, Bishop, Knight, Queen} ) {
        int constraintPieces = constraint.getConstrain(piece);
        int currentPieces = whitePieceCounts[pieceToIndexWithoutKing(piece)];


        if ( constraintPieces > currentPieces) {    //must use promotion
            int promotionsNeeded = constraintPieces - currentPieces;
            possiblePromotions -= promotionsNeeded;

            if ( possiblePromotions < 0 )  { //    => needs more promotions that available
                LOG_NOTICE << "Constraint can't be satisifed, not enought " << PIECE_NAMES.at(piece) << "s " << currentPieces << "|" << constraintPieces << endl;
                cout << errorMsg << endl;
                return false;
            }
        }

    }

    return true;
}
