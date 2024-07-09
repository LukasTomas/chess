#include "../common/Program.h"
#include "../../src/terminal/Logging.h"
#include "../../src/common/BinaryFile.h"

#include "../../src/pgn/PGN.hpp"

#include <vector>
#include <iostream>
#include <csignal>
using namespace std;

sig_atomic_t stopFlag = 0;

const int PRIVNT_EVERY = 10000;
const int MIN_RANK = 1000;
const int MIN_MOVES = 5;
const vector<string> NOT_ALLOWED_TIMECONTROL = {"60", "120", "180", "240"};


void showHelp(string programName) {
    cout << "Usage: " << programName << " <pgn file>" << endl
         << "\t\t <pgn file> - pgn file containing chess games" << endl;
}

int main( int argc, char* argv[]) {
    Program program(argv);
    program.Start();

    signal(SIGINT, [](int signum) {
        LOG_ERROR << "SIGINT received, exiting..." << endl;
        stopFlag = 1;
    });

    if (argc != 2) {
        showHelp(argv[0]);
        return 1;
    }

    const string pgnFilePath(argv[1]);
    if ( !binfile::FileExists(pgnFilePath) ) {
        LOG_ERROR << "File " << pgnFilePath << " does not exist" << endl;
        return 1;
    }

    pgnp::PGN pgn;
    pgn.FromFile(pgnFilePath);

    int gameNumber = 1;
    int lowRankGames = 0;
    int tooShortGames = 0;
    int unallowedTimecontrolGames = 0;
    int invalidGames = 0;

    int longerUnfinishedGames = 0;
    try {
        while ( !stopFlag ) {
            bool isInvalidGame = false;

            pgn.ParseNextGame();
            const string termination = pgn.GetTagValue("Termination");

            if ( pgn.GetTagValue("WhiteElo") != "?" && pgn.GetTagValue("BlackElo") != "?" ) {
                const int whiteElo = stoi(pgn.GetTagValue("WhiteElo"));
                const int blackElo = stoi(pgn.GetTagValue("BlackElo"));
                if ( whiteElo < MIN_RANK || blackElo < MIN_RANK ) {
                    lowRankGames++;
                    isInvalidGame = true;
                }
            }

            pgnp::HalfMove * moves=new pgnp::HalfMove();
            pgn.GetMoves(moves); // Get the tree of half moves
            const int length=moves->GetLength(); // Get the number of half move in the current MainLine
            if ( length < 5 ) {
                tooShortGames++;
                isInvalidGame = true;
            } else if ( termination == "Abandoned" ) {
                longerUnfinishedGames++;
                isInvalidGame = true;
            }

            const string timeControl = pgn.GetTagValue("TimeControl");
            string strippedTimeControl = timeControl.substr(0, timeControl.find('+'));
            if ( NOT_ALLOWED_TIMECONTROL.end() != find(NOT_ALLOWED_TIMECONTROL.begin(), NOT_ALLOWED_TIMECONTROL.end(), strippedTimeControl) ) {
                unallowedTimecontrolGames++;
                isInvalidGame = true;
            }

            if ( gameNumber % PRIVNT_EVERY == 0 )
                cout  << "Game n." << gameNumber << endl;

            if ( isInvalidGame )
                invalidGames++;
            gameNumber++;

            delete moves;
        }

    } catch( pgnp::NoGameFound & e ){
        cout << e.what() << endl;
        cout << "all games" << endl;
    }

    cout << endl << "=====================================" << endl;
    LOG_INFO << "Total games:\t\t\t" << gameNumber << endl;
    cout << "Low rank games:\t\t\t" << lowRankGames << endl;
    cout << "Too short games:\t\t" << tooShortGames << endl;
    cout << "Unallowed timecontrol games:\t" << unallowedTimecontrolGames << endl;
    cout << "Longer unfinished games:\t" << longerUnfinishedGames << endl;
    LOG_INFO << "Total valid games:\t\t" << (gameNumber - invalidGames) << endl;
    cout << "=====================================" << endl;

    return 0;
}
