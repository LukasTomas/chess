#include "common/Program.h"

#include "../src/common/BinaryFile.h"

#include "../src/engineCore/ChessDefinitions.h"
#include "../src/engineCore/chessComponents/Bitboard.h"
#include "../src/game/MinimaxGame.h"

#include "../src/evaluation/Constraint.h"
#include "../src/evaluation/Constraints.h"
#include "../src/evaluation/Evaluator.h"

#include "../src/terminal/ProgressShower.h"
#include "../src/terminal/Logging.h"

#include <iostream>
#include <filesystem>
#include <array>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <ctime>
#include <regex>
using namespace std;
namespace fs = std::filesystem;

static const string DESCRIPTION = "Analyze across all games for each position constraint satisfaction rate";


class BlockPrinter {
public:

    const static char BLOCK = '-';
    const static int TAB_SIZE = 3;

    const string name;

    BlockPrinter( const string & name ) : name(name) {}

    void AddLine( const string & line ) {
        string changedTabs = "\t" + line;
        changedTabs = std::regex_replace(changedTabs, std::regex("\t"), string(TAB_SIZE, ' '));
        lines.emplace_back(changedTabs);

        longestLineLength = max(longestLineLength, (int)changedTabs.size());
    }

    void PrintAll( ostream & os=cout ) {
        int headerSize = PrintHeader(os);
        for ( string & line : lines )
            os << line << endl;
        os << string(headerSize, BLOCK) << endl;
    }

private:

    vector<string> lines;
    int longestLineLength = 0;

    int PrintHeader( ostream & os=cout ) {
        int headerSize = name.size() + 2/*two spaces*/;
        int headerSizeRemains = longestLineLength - headerSize + 1*TAB_SIZE/*one tab at the end*/;
        if ( headerSizeRemains < 0 )
            throw runtime_error("Header size is negative");

        int before = headerSizeRemains / 2;
        int after = headerSizeRemains - before;

        os << string(before, BLOCK) << " " << name << " " << string(after, BLOCK) << endl;

        return longestLineLength + 1*TAB_SIZE/*one tab at the end*/;
    }

};

class Statistics {
public:
    const static int EMPTY_ARRAY_RESULT = -203984;

    Statistics( const Constraint & constraint, uint32_t numPositions, const string & dirPath ) : constraint(constraint) {
        _numPositions = numPositions;
        _statisticsFilename = dirPath + "/statistics.txt";
        _bitboardsFilename = dirPath + "/bitboards.chess";

        // errors.push_back({0, 0, 0, 0, 1});
        // errors.push_back({0, 0, 0, 1, 0});
        // errors.push_back({0, 0, 2, 0, 0});
        // games = 3;
    }

    void Add( MinimaxGame & game, bool print=false) {
        games++;

        if ( game.search.evaluator.ConstraintSatisfied() ) {
            constraintSatisfied++;
            satisfiedAfter.push_back(game.game.getGameInfo().moveNumber);
            // cout << "Constraint satisfied after " << game.game.getGameInfo().moveNumber << " moves" << endl;
        } else {
            // compute difference
            PieceCounts pieceCounts{game.game.getController().getPieceCounts(White)};
            for ( piece_t piece : {Queen, Rook, Bishop, Knight, Pawn} )
                pieceCounts[pieceToIndexWithoutKing(piece)] -= constraint.getConstrain(piece);
            errors.push_back(pieceCounts);

            if ( print ) {
                cout << "number: " << game.game.getGameInfo().moveNumber << endl;
                PrintError(pieceCounts);
            }
        }

        // add Bitboard
        uint64_t bitboard_array[12];
        game.getBoard().toBitboard(bitboard_array);
        Bitboard bitboard;
        for ( int i = 0; i < 12; i++ )
            bitboard.binPlanes[i] = bitboard_array[i];

        if ( savedBoards.count(bitboard) == 1 )
            savedBoards[bitboard]++;
        savedBoards.insert({bitboard, 1});

    }

    void TellResult() {
        TellResult(cout);
    }

    void Save() {
        // save statistics
        ofstream os(_statisticsFilename);
        TellResult(os);
        
        // save all bitboards
        ofstream o(_bitboardsFilename, ios::binary);
        for ( auto it = savedBoards.begin(); it != savedBoards.end(); it++ )
            it->first.save(o);
    }

    string GetStatisticsFilename() const {
        return _statisticsFilename;
    }

    string GetBitboardsFilename() const {
        return _bitboardsFilename;
    }

private:

    struct PieceCounts {
        array<int, MAX_ARRAY_SIZE_NO_KING> counts;

        bool operator==( const PieceCounts & other ) const {
            int sumThis = accumulate(counts.begin(), counts.end(), 0);
            int sumOther = accumulate(other.counts.begin(), other.counts.end(), 0);
            return sumThis == sumOther;
        }

        bool operator>( const PieceCounts & other ) const {
            int sumThis = accumulate(counts.begin(), counts.end(), 0);
            int sumOther = accumulate(other.counts.begin(), other.counts.end(), 0);
            return sumThis > sumOther;
        }

        bool operator<( const PieceCounts & other ) const {
            int sumThis = accumulate(counts.begin(), counts.end(), 0);
            int sumOther = accumulate(other.counts.begin(), other.counts.end(), 0);
            return sumThis < sumOther;
        }

        size_t size() const {
            return counts.size();
        }

        int & operator[]( int index ) {
            return counts[index];
        }

        int operator[]( int index ) const {
            return counts[index];
        }

        int totalCount() const {
            return accumulate(counts.begin(), counts.end(), 0);
        }

    };

    const Constraint & constraint;
    uint32_t _numPositions;
    string _statisticsFilename;
    string _bitboardsFilename;

    int games = 0;

    int constraintSatisfied = 0;
    vector<int> satisfiedAfter;

    vector<PieceCounts> errors;
    map<Bitboard, int> savedBoards;


    void TellResult( ostream & os ) {
        BlockPrinter printer("ACCURACY");

        double accuracy = 100.0 * constraintSatisfied / games;
        printer.AddLine("Satified ratio:\t" + to_string(accuracy) + "% -- " + to_string(constraintSatisfied) + " / " + to_string(games));

        vector<pair<int, int>> errorValueCounts = GetErrorValueCounts();
        if ( errorValueCounts.size() > 0 ) {
            for (const pair<int, int> & pair : errorValueCounts) {
                double differenceAccuracy = 100.0 * pair.second / games;
                printer.AddLine(to_string(pair.first) + " piece difference ratio:\t" + to_string(differenceAccuracy) + "% -- " + to_string(pair.second) + " / " + to_string(games));
            }
        }

        printer.PrintAll(os);

        PrintVectorStatistics(satisfiedAfter, "After how many moves was the constraint satisfied", os);
        

        // bitboard info
        os << "---------------------------" << endl;
        os << "Unique positions " << ((savedBoards.size() / (double)_numPositions))*100 << "% " << savedBoards.size() << "/" << _numPositions << endl;
    }

    void PrintError( const PieceCounts & error ) {
        for ( piece_t piece : {Queen, Rook, Bishop, Knight, Pawn} ) {
            int value = error[pieceToIndexWithoutKing(piece)];

            string description;
            if ( value == 0 )
                description = "OK";
            else {
                description = to_string(value);
                description += value == 1 ? " piece" : " pieces";
                if ( value > 0 )
                    description += " more";
                else
                    description += " less";
            }
            cout << PIECE_NAMES.at(piece) << ":\t" << value << " - " << description << endl;
        }
    }

    // Statistics operations
    void PrintVectorStatistics( const vector<int> & values, const string & description, ostream & os=cout ) {
        os << description << " ---------------- " << endl;
        if ( values.empty() ) {
            LOG_WARNING << "Constraint satisfied ZERO times." << endl;
            return;
        }

        os << "\tMean:\t" << Mean(values) << endl;
        os << "\tMedian:\t" << Median(values) << endl;
        os << "\tMin:\t" << Min(values) << endl;
        os << "\tMax:\t" << Max(values) << endl;

        const int k = 8;
        vector<pair<int, int>> mostCommon = MostCommon(values, k);
        os << "\tSorted " << k << " most common:\t";
        for (const pair<int, int> & pair : mostCommon)
            os << pair.first << " (" << pair.second << "), ";
        os << endl;
    }

    double Mean( const vector<int> & values ) {
        if ( values.size() == 0 )
            return EMPTY_ARRAY_RESULT;

        double sum = accumulate(values.begin(), values.end(), 0);
        return sum / values.size();
    }

    int Median( const vector<int> & values ) {
        if ( values.size() == 0 )
            return EMPTY_ARRAY_RESULT;

        vector<int> sorted = values;
        sort(sorted.begin(), sorted.end());
        return sorted[sorted.size() / 2];
    }

    int Min( const vector<int> & values ) {
        if ( values.size() == 0 )
            return EMPTY_ARRAY_RESULT;

        return *min_element(values.begin(), values.end());
    }

    int Max( const vector<int> & values ) {
        if ( values.size() == 0 )
            return EMPTY_ARRAY_RESULT;

        return *max_element(values.begin(), values.end());
    }

    vector<pair<int, int>> MostCommon( const vector<int> & values, int k=8) {
        vector<int> sorted = values;
        sort(sorted.begin(), sorted.end());

        vector<pair<int, int>> mostCommon = GetValueCounts(sorted);

        sort(mostCommon.begin(), mostCommon.end(), [](const pair<int, int> & a, const pair<int, int> & b) {
            return a.second > b.second;
        });


        // drop (size - k) elements
        if ( mostCommon.size() > k )
            mostCommon.erase(mostCommon.begin() + k, mostCommon.end());

        return /*k*/mostCommon;
        return {};
    }


    template<typename T>
    vector<pair<T, int>> GetValueCounts(const vector<T>& values) {
        vector<T> sorted = values;
        sort(sorted.begin(), sorted.end());

        vector<pair<T, int>> valueCounts;

        T currentValue = sorted[0];
        pair<T, int> currentPair = make_pair(currentValue, 1);

        for (int i = 1; i < sorted.size(); i++) {
            if (currentValue == sorted[i])
                currentPair.second++;
            else {
                // new value
                valueCounts.push_back(currentPair);
                currentValue = sorted[i];
                currentPair = make_pair(currentValue, 1);
            }
        }

        // add last value
        valueCounts.push_back(currentPair);

        return valueCounts;
    }

    string pieceCountToStr( const PieceCounts & pieceCounts ) {
        string str = "";
        for (int i = 0; i < pieceCounts.size(); ++i) {
            str += to_string(pieceCounts[i]);
            if ( i != pieceCounts.size() - 1 )
                str += ", ";
        }
        return str;
    }

    vector<pair<int, int>> GetErrorValueCounts() {
        if ( errors.size() == 0 )
            return {};

        vector<PieceCounts> absoluteValueErrors;
        for (const PieceCounts & pieceCounts : errors) {

            PieceCounts absoluteValueError;

            for (int i = 0; i < pieceCounts.size(); ++i)
                absoluteValueError[i] = abs(pieceCounts[i]);

            absoluteValueErrors.push_back(absoluteValueError);
        }



        vector<pair<PieceCounts, int>> errorCounts = GetValueCounts(absoluteValueErrors);
        // cout << "ErrorCounts" << endl;
        // for ( pair<PieceCounts, int> & p : errorCounts )
        //     cout << pieceCountToStr(p.first) << " (" << p.second << "), " << endl;

        vector<pair<int, int>> errorValueCounts;


        // DOES: first -> sum of error pieces, second -> number of occurences
        for (const pair<PieceCounts, int> & pair : errorCounts) {
            int sum = pair.first.totalCount();
            errorValueCounts.push_back(make_pair(sum, pair.second));
        }
        // cout << "ErrorValueCounts" << endl;
        // for ( pair<int, int> & p : errorValueCounts )
        //     cout << p.first << " (" << p.second << "), " << endl;

        sort(errorValueCounts.begin(), errorValueCounts.end(), [](const pair<int, int> & a, const pair<int, int> & b) {
            return a.first < b.first;
        });

        return errorValueCounts;
    }

};


const string STATISTICS_MAIN_DIR = "statistics";
string dirName( const Constraint & c ) {
    string dirName;

    auto currentTimePoint = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(currentTimePoint);
    std::tm* timeInfo = std::localtime(&time);

    stringstream ss;
    ss << std::put_time(timeInfo, "%Y-%m-%d");
    string date = ss.str();

    dirName = c.toString() + "_" + date;

    for ( int i = 0; ; i++ ) {
        if ( i != 0 )
            dirName = c.toString() + "_" + date + "_" + to_string(i);

        if ( !binfile::FileExists("../" + STATISTICS_MAIN_DIR + "/" + dirName) )
            break;
    }

    return "../" + STATISTICS_MAIN_DIR + "/" + dirName;
} 

int _main( uint32_t numPositions, const Constraint & constraint ) {
    srand(time(nullptr));

    // create dir for stat
    const string dirPath = dirName(constraint);
    cout << "Saving stats to " << dirPath << " directory" << endl;
    fs::create_directory(dirPath);

    Statistics statistics(constraint, numPositions, dirPath);
    cout << "Generating " << numPositions << " positions to file '" << statistics.GetStatisticsFilename() << "'" << endl;


    ProgressShower progressShower;
    for (int i = 0; i < numPositions; ++i) {

        MinimaxGame game(constraint);
        while( game.makeMove_2() ) {}

        statistics.Add(game, false);

        cout << "moves: " << game.game.getGameInfo().moveNumber << endl;
        cout << game.getBoard().toString() << endl << endl;
        
        progressShower.showProgress(i + 1, numPositions);
    }
    cout << endl;

    statistics.TellResult();

    LOG_INFO << "Saving statistics to file " << statistics.GetStatisticsFilename() << ", bitboards to " << statistics.GetBitboardsFilename() << endl;
    statistics.Save();

    return 0;
}

// ---------------------------------------------------------------------------------------------------------------------

int loadConstraint(const string & cstString) {
    for(int i = 0; i < constrains.size(); i++) {
        if (constrains[i].toString() == cstString) {
            cout << "Loaded constraint: " << constrains[i].toString() << endl;
            return i;
        }
    }

    LOG_ERROR << "Constraint '" << cstString << "' is not allowed" << endl;
    return -1;
}

int main(int argc, char * argv[]) {
    Program program(argc, argv, DESCRIPTION);
    program.helper.AddOption("numPositions", "The number of positions to generate", true);
    program.helper.AddOption("constraint", "5-tuple (Queens, Rooks, Bishops, Knights, Pawns)", true);
    program.Start();


    uint32_t numPositions;
    try {
        numPositions = stoi(argv[2]);

        if (numPositions <= 0) {
            LOG_ERROR << "Number of positions must be greater than 0." << endl;
            program.helper.ShowHelp();
            return 2;
        }
    } catch( exception const & e ) {
        LOG_ERROR << "Couldn't convert string '" + string(argv[2]) + "' to int" << endl;
        program.helper.ShowHelp();
        return 2;
    }


    int cstIndex = loadConstraint(argv[4]);
    if( cstIndex == -1 ) {
        LOG_ERROR << "Couldn't load constraint" << endl;
        program.helper.ShowHelp();
        return 3;
    }
    const Constraint & constraint = constrains[cstIndex];

    return _main(numPositions, constraint);
}
