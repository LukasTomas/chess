// 3. - Analyze across all games for each position constraint satisfaction rate

#include "../common/Program.h"
#include "../common/Helper.h"
#include "../common/Errors.h"

#include "../../src/terminal/Logging.h"
#include "../../src/common/BinaryFile.h"
#include "../../src/dataset/DatasetIterator.h"
#include "../../src/dataset/parsing/ConstraintGameData.h"
#include "../../src/dataset/BitboardAnalyzed.h"

#include "../../src/evaluation/Constraints.h"
#include "../../src/engineCore/chessComponents/Board.h"
#include "../../src/engineCore/chessComponents/Bitboard.h"

#include <iostream>
#include <map>
using namespace std;

static const string DESCRIPTION = "Analyze across all games for each position constraint satisfaction rate";
static const string OUT_EXTENSION = ".analyzed";

class Analyzier {
public:

    const int MIN_RANK = 1000;
    const int MIN_MOVES = 5;
    const vector<string> NOT_ALLOWED_TIMECONTROL = {"60", "120", "180", "240"};



    Analyzier() {}

    void Analyze( ParsedGame * game, ConstraintGameData * constraintGameData ) {
        if ( game->whiteElo < MIN_RANK || game->blackElo < MIN_RANK || game->whiteElo == ParsedGame::UNKNOWN_ELO || game->blackElo == ParsedGame::UNKNOWN_ELO ) {
            // cout << "low rank game" << endl;
            return;
        }

        if ( game->moves.size() < MIN_MOVES ) {
            // cout << "too short game" << endl;
            return;
        }

        string strippedTimeControl = game->timeControl.substr(0, game->timeControl.find('+'));
        if ( find(NOT_ALLOWED_TIMECONTROL.begin(), NOT_ALLOWED_TIMECONTROL.end(), strippedTimeControl) != NOT_ALLOWED_TIMECONTROL.end() ) {
            // cout << "unallowed timecontrol" << endl;
            return;
        }

        vector<int> constraintsLastSatisfied(constrains.size(), 0);
        GetConstraintLastSatisfied(game, constraintsLastSatisfied);

        // for ( int i = 0; i < constrains.size(); i++ )
        //     cout << i << ", " << constraintsLastSatisfied[i] << endl;

        int moveNumber = 0;
        while ( game->isNext() ) {
            game->doNextMove();
            moveNumber++;

            uint64_t bitboard[12] = {0};
            game->getBoard().toBitboard(bitboard);
            Bitboard bitboardStruct;
            for ( int i = 0; i < 12; i++ ) {
                bitboardStruct.binPlanes[i] = bitboard[i];

                // cout << endl << endl;
                // cout << game->getBoard().toString() << endl;
                // cout << "bitboard: " << endl;
                // for ( int j = 0; j < 64; j++ ) {
                //     if ( j % 8 == 0 )
                //         cout << endl;
                //     cout << (bitboard[i] & (1ULL << j) ? "1" : "0");
                // }
            }

            vector<satisfiedInfo> & vec = positionConstraintSatisfaction[bitboardStruct];
            if ( vec.size() == 0 )
                vec.resize(constrains.size(), {0, 0});

            for ( int i = 0; i < constrains.size(); i++ ) {
                if ( moveNumber <= constraintsLastSatisfied[i] )
                    vec[i].satisfied++;
                else
                    vec[i].unsatisfied++;

                // if ( (vec[i].satisfied > 100000 ) || (vec[i].unsatisfied > 500000) )
                //     cout << "satisfied: " << vec[i].satisfied << ", unsatisfied: " << vec[i].unsatisfied << endl;
            }

        }
    }

    void Finish( const string & outputFilePath ) {
        ofstream os(outputFilePath, ios::binary);

        for ( auto & pair : positionConstraintSatisfaction ) {
            BitboardAnalyzed bitboardAnalyzed;
            bitboardAnalyzed._bitboard = pair.first;

            // save bitboard
            // then save probabilities for each constraint

            vector<satisfiedInfo> & vec = pair.second;
            for ( int i = 0; i < constrains.size(); i++ ) {
                satisfiedInfo & info = vec[i];
                double probability = (double)info.satisfied / (info.satisfied + info.unsatisfied);
                bitboardAnalyzed._constraintSatisfiedProbab[i] = probability;

                // if ( probability > 0.3 )
                //     cout << probability << endl;
            }

            bitboardAnalyzed.save(os);
        }

        cout << "total bitboard: " << positionConstraintSatisfaction.size() << endl;
        // 17 698 265
        // 17 780 538
        // 17 698 265
        //  9 803 755
    }

private:

    struct satisfiedInfo {
        int satisfied;
        int unsatisfied;
    };

    // for each position: for each constraint satisfiedInfo
    map<Bitboard, vector<satisfiedInfo> > positionConstraintSatisfaction;

    void GetConstraintLastSatisfied( ParsedGame * parsedGame, vector<int> & constraintsLastSatisfied ) {
        for ( int moveNumber = 1; parsedGame->isNext(); moveNumber++ ) {
            parsedGame->doNextMove();

            for ( int i = 0; i < constrains.size(); i++ ) {
                if( parsedGame->getController().getPieceCounts(White) == constrains[i].constraints ) {
                    //cout << "SATISFIED -----------------------------------" << moveNumber << endl;
                    constraintsLastSatisfied[i] = moveNumber;
                }
            }
        }

        parsedGame->reset();
    }

};

int _main( const string & inFilePath, const string & outFilePath ) {
    Analyzier analyzier;
    DatasetIterator<ConstraintGameData> loader(inFilePath);
    int gameNumber = 0;

    const int logEvery = 10000;

    while ( !loader.IsEnd() ) {
        if ( gameNumber % logEvery == 0 )
            cout << "Loading game number " << gameNumber << endl;
        gameNumber++;

        ParsedGame * game = loader.LoadNext();
        if ( game == nullptr )
            break;

        analyzier.Analyze(game, loader.CurrentAdditionalData());
    }

    analyzier.Finish(outFilePath);

    LOG_INFO << "Successfully loaded all(" << gameNumber << ") games" << endl;

    // ------------------------------------------------------------
    // test
    // BitboardAnalyzed bitboardAnalyzed;
    // ifstream is(outFilePath, ios::binary);
    // while ( bitboardAnalyzed.load(is) ) {
    //     // cout << bitboardAnalyzed._bitboard.toString() << endl;
    //     for ( int i = 0; i < constrains.size(); i++ ) {
    //         if ( bitboardAnalyzed._constraintSatisfiedProbab[i] > 0.3  )
    //             cout << i << ": " << bitboardAnalyzed._constraintSatisfiedProbab[i] << endl;
    //     }
    // }

    return 0;
}

// ------------------------------------------------------------

int main(int argc, char* argv[]) {
    Program program(argc, argv, DESCRIPTION);
    program.helper.AddOption("input", "binary parsed chess game with constraints - .constraint extension", true);
    program.Start();

    string input(argv[2]);
    if( !binfile::FileExists(input) ) {
        LOG_ERROR << "File '" << input << "' does not exist" << endl;
        program.helper.ShowHelp();
        return FILE_NOT_EXIST;
    }

    string output = input + OUT_EXTENSION;

    return _main(input, output);
}
