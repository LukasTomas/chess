#include "Search.h"

double Search::minimax( int depth ) {
    if ( depth == 0 ) {
        //if ( game.playingSideHasCheck() )
        //    return NEGATIVE_INF;

        if ( evaluator.ConstraintSatisfied() )
            return 1;

        return evaluator.Evaluate( game.sidePlaying() );
    }

    vector<Move> moves;
    game.getLegalMoves(moves);
    if ( moves.empty() )
        return 0;

    double bestEvaluation = NEGATIVE_INF;

    for ( Move & move : moves ) {
        game.doMove(move);
        double evaluation = minimax(depth-1);
        game.undoMove();

        if ( evaluation > bestEvaluation )
            bestEvaluation = evaluation;
    }

    return bestEvaluation;
}

int Search::AlphaBeta( int depth, int alpha, int beta ) {
    if ( depth == 0 )
        return evaluator.Evaluate( game.sidePlaying() );

    vector<Move> moves;
    game.getLegalMoves(moves);
    if ( moves.empty() ) {
        if ( game.playingSideHasCheck() )
            return NEGATIVE_INF;

        return 0;
    }

    for ( Move & move : moves ) {
        game.doMove(move);
        int evaluation = -AlphaBeta(depth-1, -beta, -alpha); // Negamax
        game.undoMove();

        if ( evaluation >= beta )
            return beta;

        if ( evaluation > alpha )
            alpha = evaluation;
    }

    return alpha;
}
