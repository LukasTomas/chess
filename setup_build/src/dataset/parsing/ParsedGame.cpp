#include "ParsedGame.h"

#include "../../common/BinaryFile.h"

using namespace std;

// CONSTRUCTORS ------------------------------------------------------------------------------------


ParsedGame::ParsedGame( const string & resultStr, const string & whiteEloStr, const string & blackEloStr,
            const string & timeControl, const string & eco, const string & opening, const vector<Move> & moves ) {

    if ( resultStr == "1-0" )
        result = 1;
    else if ( resultStr == "0-1" )
        result = 0;
    else if ( resultStr == "1/2-1/2" )
        result = 0.5;
    else
        throw invalid_argument("result is not valid: " + resultStr);


    whiteElo = (whiteEloStr == "?" || whiteEloStr == "") ? UNKNOWN_ELO : stoi(whiteEloStr);
    blackElo = (blackEloStr == "?" || blackEloStr == "") ? UNKNOWN_ELO : stoi(blackEloStr);

    this->timeControl = timeControl;
    this->eco = eco;
    this->opening = opening;

    this->moves = moves;
}


// PUBLIC METHODS ----------------------------------------------------------------------------------

void ParsedGame::reset() {
    controller.reset();
    moveNumber = 0;
}

const Board & ParsedGame::getBoard() const {
    return controller.board;
}

const BoardController & ParsedGame::getController() const {
    return controller;
}

ostream & ParsedGame::save( ostream & os ) const {
    os.write(reinterpret_cast<const char*>(&result), sizeof(result));
    os.write(reinterpret_cast<const char*>(&whiteElo), sizeof(whiteElo));
    os.write(reinterpret_cast<const char*>(&blackElo), sizeof(blackElo));
    binfile::SaveString(os, timeControl);
    binfile::SaveString(os, eco);
    binfile::SaveString(os, opening);

    const size_t movesCount = moves.size();
    os.write(reinterpret_cast<const char *>(&movesCount), sizeof(movesCount));
    for ( auto it = moves.begin(); it != moves.end(); ++it )
        it->save(os);

    return os;
}

istream & ParsedGame::load(istream & is) {
    if (is.eof() || is.fail() || is.bad())
        return is;
    reset();

    is.read(reinterpret_cast<char*>(&result), sizeof(result));
    is.read(reinterpret_cast<char*>(&whiteElo), sizeof(whiteElo));
    is.read(reinterpret_cast<char*>(&blackElo), sizeof(blackElo));

    binfile::LoadString(is, timeControl);
    binfile::LoadString(is, eco);
    binfile::LoadString(is, opening);

    size_t movesCount;
    is.read(reinterpret_cast<char*>(&movesCount), sizeof(movesCount));
    moves.resize(movesCount);

    for (size_t i = 0; i < movesCount; i++)
        moves[i].load(is);

    return is;
}

string ParsedGame::toString( bool printMoves ) const {
    stringstream ss;

    ss << "result is " << result << endl;
    ss << "whiteElo is " << whiteElo << endl;
    ss << "blackElo is " << blackElo << endl;
    ss << "time control " << timeControl << endl;
    ss << "eco " << eco << endl;
    ss << "opening " << opening << endl;
    ss << "moves: " << moves.size() <<  endl;

    if ( printMoves ) {
        for ( auto it = moves.begin(); it != moves.end(); ++it )
            ss << *it;
    }

    return ss.str();
}

ostream & operator<<( ostream & o, const ParsedGame & game ) {
    o << game.toString();
    return o;
}
