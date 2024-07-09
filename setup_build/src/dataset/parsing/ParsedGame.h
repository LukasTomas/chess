#pragma once

#include "../../engineCore/chessComponents/Move.h"
#include "../../engineCore/chessLogic/BoardController.h"

#include <iostream>
#include <list>
#include <vector>
#include <stdexcept>

class ParsedGame {
public:

    static const int UNKNOWN_ELO = -1;

    int result;

    int whiteElo;
    int blackElo;

    std::string timeControl;
    std::string eco, opening;

    std::vector<Move> moves;

    ParsedGame() = default;
    ParsedGame( const std::string & result, const std::string & whiteElo, const std::string & blackElo,
                const std::string & timeControl, const std::string & eco, const std::string & opening, const std::vector<Move> & moves );

    ParsedGame & operator=( const ParsedGame & game ) {
        if (this != &game) {
            moveNumber = game.moveNumber;
            result = game.result;
            whiteElo = game.whiteElo;
            blackElo = game.blackElo;
            timeControl = game.timeControl;
            eco = game.eco;
            opening = game.opening;
            moves = game.moves;
        }

        return *this;
    }

    inline void doNextMove() {
        if ( moves.size() == 0 ) {
            throw logic_error("ZERO MOVES GAMES");
        }
        if ( !isNext() )
            throw logic_error("parsed game - no moves left - time control: " + timeControl);

        controller.doMove(moves[moveNumber]);
        moveNumber++;
    }

    inline bool isNext() const {
        return moves.size() != 0 && moveNumber <= moves.size()-1;
    }

    void reset();

    const Board & getBoard() const;
    const BoardController & getController() const;

    std::ostream & save( std::ostream & os ) const;
    std::istream & load( std::istream & is );

    std::string toString( bool printMoves=false ) const;
    friend ostream & operator<<( ostream & o, const ParsedGame & game );

    int moveNumber = 0;
private:

    BoardController controller;

};
