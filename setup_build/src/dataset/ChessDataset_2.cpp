#include "ChessDataset_2.h"

#include <limits>


const vector<string> ChessDataset_2::NOT_ALLOWED_TIMECONTROL = {"60", "120", "180", "240"};

// CONSTRUCTORS ------------------------------------------------------------------------------------------------

ChessDataset_2::ChessDataset_2( const string & filePath ) : path(filePath), iterator(filePath) {
    int invalidEloGames = 0;
    int invalidTimeControlGames = 0;

    ParsedGame * game;
    while( !iterator.IsEnd() ) {
        if ( (game = iterator.LoadNext()) == nullptr )
            break;

        gamesCount++;

        if ( game->whiteElo < ELO_THRESHOLD || game->blackElo < ELO_THRESHOLD ) {
            invalidEloGames++;
            continue;
        }

        string timeControl = game->timeControl.substr(0, game->timeControl.find('+'));
        if ( NOT_ALLOWED_TIMECONTROL.end() != find(NOT_ALLOWED_TIMECONTROL.begin(), NOT_ALLOWED_TIMECONTROL.end(), timeControl) ) {
            invalidTimeControlGames++;
            continue;
        }

        // Only count VALID games moves
        movesCount += game->moves.size();
    }

    datasetSize = movesCount * constrains.size();
    backToStart();

    cout << "Total games:\t\t\t" << gamesCount << endl;
    cout << "Invalid elo games:\t\t" << invalidEloGames << endl;
    cout << "Invalid time control games:\t" << invalidTimeControlGames << endl;
    cout << "-----------------------------------------" << endl;
    gamesCount -= invalidEloGames + invalidTimeControlGames;
    cout << "Valid games:\t\t\t" << gamesCount << endl;
    cout << "Moves:\t\t\t\t" << movesCount << endl;
    cout << "Constrains:\t\t\t" << constrains.size() << endl;
    cout << "TOTAL DATASET SIZE:\t\t" << datasetSize << endl;
    cout << "-----------------------------------------" << endl;
}

ChessDataset_2::ChessDataset_2(const ChessDataset_2 & dataset)
: path(dataset.path), iterator(dataset.path)  {
    // cout << "copy constructor" << endl;

    gamesCount = dataset.gamesCount;
    movesCount = dataset.movesCount;
    datasetSize = dataset.datasetSize;

    // cout << "others index " << dataset.datasetIndex << endl;
    if ( dataset.datasetIndex != 0 )
        throw logic_error("copy constructor with dataset index != 0");

    // cout << endl;
}

// PUBLIC METHODS ----------------------------------------------------------------------------------------------

//42090555
torch::data::Example<> ChessDataset_2::get(size_t index) {
    if ( (gameIndex + 1) == gamesCount || (index-1) == datasetSize ) {
        backToStart();
    }

    ParsedGame * game = iterator.CurrentGame();
    if ( constraintIndex == 0 ) {
        if ( game == nullptr || !game->isNext() ) {
            game = loadNextGame();
            gameIndex++;
        }

        game->doNextMove();
    }

    // DOESN'T share memory with board array
    torch::Tensor boardTensor = torch::zeros({6*2, 8, 8}, torch::kUInt8);
    game->getBoard().toBitboardsTensor(boardTensor);

    double result = getResult();
    torch::Tensor resultTensor = torch::full({1}, result, torch::kFloat64);

    constraintIndex++;
    datasetIndex++;
    if ( constraintIndex == constrains.size() )
        constraintIndex = 0;

    // if ( datasetIndex == 500000 ) {
    //     backToStart();
    // }

    torch::Tensor constraintAlgebraic = torch::zeros({5, 8, 8}, torch::kUInt8);
    constrains[constraintIndex].toAlgebraicTensor(constraintAlgebraic);

    // cout << "board tensor: \n" << boardTensor << endl << endl;
    // cout << "constraint tensor: " << constraintTensor << endl << endl;
    // cout << "result tensor: " << resultTensor << endl << endl;
    // cout << "SATISFIED: " << constraintEvaluation << "----------------------------------------------------------" << endl;
    // auto concatenated_tensor = torch::cat({boardTensor.view({1, -1}), constraintTensor.view({1, -1})}, 1);
    auto input = torch::cat({boardTensor, constraintAlgebraic}, 0);
    return {input, resultTensor};
}


// returns max number because dataset size is not known beforehand
torch::optional<size_t> ChessDataset_2::size() const {
    return datasetSize;
    // return 500000;
}

// PRIVATE METHODS --------------------------------------------------------------------------------------------

double ChessDataset_2::getResult() const {
    // Constraint fullfilment constants
    const double currentFullfilmentConstant = 0.6;
    const double futureFullfilmentConstant = 1 - currentFullfilmentConstant;

    array<int, MAX_ARRAY_SIZE_NO_KING> pieceCounts = iterator.CurrentGame()->getController().getPieceCounts(White);
    double currentFullfillment = constrains[constraintIndex].evaluate(pieceCounts);

    int futureConstraintFullfilment = iterator.CurrentAdditionalData()->_data[constraintIndex] != 0 ? 1 : 0;

    double composedResult = (currentFullfillment != 0) ?
        (currentFullfilmentConstant*currentFullfillment + futureFullfilmentConstant*futureConstraintFullfilment)
        : 0;
    // double composedResult = futureConstraintFullfilment;

    return composedResult;
}

void ChessDataset_2::backToStart() {
    // cout << "End of the file reached" << endl;
    iterator.Reset();

    gameIndex = 0;
    constraintIndex = 0;
    datasetIndex = 0;
}

ParsedGame * ChessDataset_2::loadNextGame() {
    ParsedGame * game;
    while( (game = iterator.LoadNext()) != nullptr ) {

        if ( game->whiteElo < ELO_THRESHOLD || game->blackElo < ELO_THRESHOLD )
            continue;

        string timeControl = game->timeControl.substr(0, game->timeControl.find('+'));
        if ( NOT_ALLOWED_TIMECONTROL.end() != find(NOT_ALLOWED_TIMECONTROL.begin(), NOT_ALLOWED_TIMECONTROL.end(), timeControl) ) {
            // cout << "skipping game with time control: " << game->timeControl << endl;
            continue;
        }

        return game;
    }

    return nullptr;
}
