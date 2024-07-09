#ifndef __ABSTRACT_GAME_H__
#define __ABSTRACT_GAME_H__

#include "../engineCore/chessComponents/Board.h"

class AbstractGame {
public:

    virtual bool makeMove() = 0;
    virtual const Board & getBoard() const = 0;

};

#endif
