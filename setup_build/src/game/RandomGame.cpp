#include "RandomGame.h"

#include <unistd.h>
#include <ctime>

RandomGame::RandomGame() : rules(game.getBoard()) {
    srand(time(nullptr));
}

RandomGame::RandomGame( Board & board ) : game(Game(board)), rules(game.getBoard())  {
    srand(time(nullptr));
}

bool RandomGame::makeMove() {
    vector<Move> moves;
    bool end = game.getLegalMoves(moves);
    if ( end )
        return false;

    Move & selectedMove = moves[rand() % moves.size()];
    bool moveMade = game.doMove(selectedMove);
    if ( !moveMade )
        throw logic_error("legal move could no have been made");

   return true;
}

const Board & RandomGame::getBoard() const {
    return game.getBoard();
}
