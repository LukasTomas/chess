#ifndef __MINIMAX_H__
#define __MINIMAX_H__

#include "AbstractGame.h"
#include "../engineCore/chessLogic/Game.h"
#include "../engineCore/chessLogic/Rules.h"
#include "../evaluation/Search.h"
#include "../evaluation/Constraint.h"

class MinimaxGame : public AbstractGame {
public:

    Game game;
    Search search;

    MinimaxGame( const Constraint & constraint );

    bool makeMove();
    bool makeMove_2();
    const Board & getBoard() const;

private:
    const Constraint & constraint;

    Rules rules;

    bool constraintSatisfied = false;

    bool isConstraintSatisfiable();

};

#endif
