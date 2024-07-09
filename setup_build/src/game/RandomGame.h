#ifndef __RANDOMGAME_H__
#define __RANDOMGAME_H__

#include "AbstractGame.h"
#include "../engineCore/chessLogic/Game.h"
#include "../engineCore/chessLogic/Rules.h"

class RandomGame : public AbstractGame {
public:

    Game game;

    RandomGame();
    RandomGame( Board & board );

    bool makeMove();
    const Board & getBoard() const;

private:
    Rules rules;
};

#endif
