// 1. Ads constraint fullfillment to a dataset for each constraint specified in

#include "../../src/dataset/DatasetIterator.h"
#include "../../src/dataset/parsing/ParsedGame.h"
#include "../../src/dataset/parsing/ConstraintGameData.h"

#include "../../src/evaluation/Constraints.h"

#include "../../src/terminal/Logging.h"
#include "../../src/terminal/DrawingChars.h"

#include "../../common/BinaryFile.h"

#include <iostream>
#include <fstream>
#include <chrono>
using namespace std;

const string DESCRIPTION = "Adds constraint fullfillment to a dataset for each constraint specified in.";

class ConstraintAdd {
public:
    static const int LOG_EVERY = 10000;

    ConstraintAdd( const string & inputFilename, const string & outputFilename )
        : is(inputFilename, ios::binary), os(outputFilename, ios::binary) {
        constraintSatisfied.resize(constrains.size(), 0);
    }

    void WriteHeader() {
        // write constrains to the file as a header
        const int constraintsCount = constrains.size();
        cout << "Writing " << constraintsCount << " constrains to the file as a header" << endl;

        os.write(reinterpret_cast<const char*>(&constraintsCount), sizeof(constraintsCount));
        for ( int i = 0; i < constraintsCount; i++ )
            constrains[i].save(os);
    }

    void AddConstraints() {
        int gamesCount = 0;
        while ( game.load(is) ) {
            gamesCount++;
            if ( gamesCount % LOG_EVERY == 0 )
                cout << "Games processed: " << gamesCount << endl;

            GetConstraintResults();

            // save all constraint results
            os.write(reinterpret_cast<const char*>(constraintSatisfied.data()), constrains.size()*sizeof(int));
            //save game
            game.save(os);
        }

        LOG_INFO << "Successfully processed " << gamesCount << " games" << endl;
    }

    void GetConstraintResults() {
        fill(constraintSatisfied.begin(), constraintSatisfied.end(), 0);

        for ( int moveNumber = 1; game.isNext(); moveNumber++ ) {
            game.doNextMove();

            for ( int i = 0; i < constrains.size(); i++ ) {
                // skip already satisfied constrains
                if ( constraintSatisfied[i] != 0 )
                    continue;

                if( game.getController().getPieceCounts(White) == constrains[i].constraints ) {
                    //cout << "SATISFIED -----------------------------------" << moveNumber << endl;
                    constraintSatisfied[i] = moveNumber;
                }
            }
        }
    }

private:

    ifstream is;
    ofstream os;

    vector<int> constraintSatisfied;
    ParsedGame game;

};

class Helper {
public:

    static const int TAB_SIZE = 4;

    Helper( const string & name, const string & description ) : _name(name), _description(description) {
        options.push_back({"help", "will show this help", false});
        _startTime = chrono::high_resolution_clock::now();
    }

    ~Helper() {
        auto endTime = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(endTime - _startTime).count();
        LOG_NOTICE << "Execution time: " << duration << "ms" << endl;
    }

    void AddOption( const string & option, const string & description, bool required = false ) {
        if ( option.length() > longestOption)
            longestOption = option.length();

        options.push_back({option, description, required});
    }

    void ShowHelp() const {
        LOG_INFO << "Usage: " << _name;
        for ( const option & o : options ) {
            LOG_INFO << " | ";
            if ( !o.required )
                LOG_INFO << "[-" << o.name << "]";
            else
                LOG_INFO << "-" << o.name;
        }
        LOG_INFO << endl;

        int tabs = longestOption / TAB_SIZE;

        for ( const option & o : options ) {
            int optionTabs = tabs - o.name.length() / TAB_SIZE + 1;
            string tabsStr(optionTabs, '\t');

            LOG_INFO << "\t-" << o.name << tabsStr << " - " << o.description << endl;
        }
    }

    void ShowStart() const {
        LOG_NOTICE << "\t\t" << _name << endl;
        for ( int i = 0; i < 40; i++ )
            LOG_NOTICE << boxDrwaing::LIGHT_HORIZONTAL;
        LOG_NOTICE << endl << endl;
    }

private:

    struct option {
        string name;
        string description;
        bool required;
    };

    string _name, _description;
    vector<option> options;

    int longestOption = 0;

    chrono::time_point<std::chrono::high_resolution_clock> _startTime;

};


int main(int argc, char* argv[]) {
    Helper helper(argv[0], DESCRIPTION);
    helper.AddOption("test", "will also test loading the dataset", false);
    helper.AddOption("only-test", "will only test loading the dataset", false);

    helper.ShowStart();

    if ( argc != 2 && argc != 3 ) {
        LOG_ERROR << "Invalid number of arguments: " << (argc - 1) << endl;
        helper.ShowHelp();
        return 1;
    }

    // check for help
    if ( string(argv[1]) == "-help" ) {
        helper.ShowHelp();
        return 0;
    }

    bool test = false;
    bool onlyTest = false;
    if ( argc == 3 ) {
        string option(argv[2]);
        if ( option != "test" && option != "only-test" ) {
            LOG_ERROR << "Invalid option '" << option << "'" << endl;
            helper.ShowHelp();
            return 2;
        }
        test = true;
        onlyTest = option == "only-test";
    }

    LOG_NOTICE << "Constraints set: " << endl;
    for ( auto it = constrains.begin(); it != constrains.end(); ++it )
        LOG_NOTICE << it->toString() << endl;
    LOG_NOTICE << endl;

    const string datasetFilename(argv[1]);
    const string newDatasetFilename = datasetFilename + ".constraints";

    string testDatasetFilename = newDatasetFilename;
    if ( onlyTest ) {
        testDatasetFilename = datasetFilename;
    } else {
        LOG_NOTICE << "Will create new dataset file '" << newDatasetFilename << "'" << endl;
        cout << "Loading dataset file '" << datasetFilename << "' ..." << endl;

        if ( !binfile::FileExists(datasetFilename) ) {
            LOG_ERROR << "File '" << datasetFilename << "' does not exist" << endl;
            return 3;
        }

        ConstraintAdd constraintAdd(datasetFilename, newDatasetFilename);
        constraintAdd.WriteHeader();
        constraintAdd.AddConstraints();
    }

    if ( test ) {
        if ( !binfile::FileExists(testDatasetFilename) ) {
            LOG_ERROR << "Tested dataset '" << testDatasetFilename << "' does not exist" << endl;
            return 4;
        }

        LOG_NOTICE << endl << "Testing loading dataset '" << testDatasetFilename << "'" << endl;
        DatasetIterator<ConstraintGameData> loader(testDatasetFilename);

        LOG_NOTICE << "Constraints loaded: " << endl;
        for ( const Constraint & c : loader.GetConstraints() )
            LOG_NOTICE << c.toString() << endl;
        LOG_NOTICE << endl;

        int gameNumber = 0;

        while ( !loader.IsEnd() ) {
            if ( gameNumber % ConstraintAdd::LOG_EVERY == 0 )
                cout << "Loading game number " << gameNumber << endl;
            gameNumber++;

            ParsedGame * game = loader.LoadNext();
            if ( game == nullptr )
                break;

            // Try to do all moves, so we can test a little if the game is valid
            while ( game->isNext() )
                game->doNextMove();
        }

        LOG_INFO << "Successfully loaded all(" << gameNumber << ") games" << endl;
    }

    return 0;
}
