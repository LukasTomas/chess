#ifndef __SEARCH_H__
#define __SEARCH_H__

#include "Evaluator.h"
#include "../engineCore/chessLogic/Game.h"

class Search {
public:

    static const int INF = 1000000;
    static const int NEGATIVE_INF = -INF;

    Evaluator evaluator;

    Search (Game & game, const Constraint & constraint) : game(game), evaluator(game.getBoard(), constraint) { }

    double minimax (int depth);
    int AlphaBeta (int depth, int alpha, int beta);

private:
    Game & game;
};


#endif  // __SEARCH_H__
