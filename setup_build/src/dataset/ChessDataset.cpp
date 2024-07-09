#include "ChessDataset.h"

#include <limits>

//42090555
torch::data::Example<> ChessDataset::get(size_t index) {
    // if ( index % 1000000 == 0 /*|| index > 41590000*/)
    //     cout << "index: " << index << " | " << datasetSize << endl;

    //cout << "gameIndex: " << gameIndex << " | " << gamesCount << endl;
    if ( (gameIndex + 1) == gamesCount || (index-1) == datasetSize ) {
        backToStart();
    }

    if ( constraintIndex == 0 ) {
        if ( !game.isNext() ) {
            if (!(i >> game) )
                throw logic_error("no moves left");

            gameIndex++;
            //cout << "\t\t ---------------------------------------- NEXT GAME ----------------------------------------" << endl;
        }

        game.doNextMove();
    }

    // cout << "current constraint: (" << constraintIndex << ")" << constrains[constraintIndex].toString() << endl;

    // WARNING tensor sharing memory with board array
    torch::Tensor sharedBoardTensor = game.getBoard().get2dTensor();
    // // because of batches making i need to copy the board tensor - board array (so board tensor) is updating
    torch::Tensor boardTensor = sharedBoardTensor.clone();

    array<int, MAX_ARRAY_SIZE_NO_KING> pieceCounts = game.getController().getPieceCounts(White);
    double constraintEvaluation = constrains[constraintIndex].evaluate(pieceCounts);

    // cout << "\t constraint evaluation: " << constraintEvaluation << endl;
    // cout << "\t game result: " << game.result << endl;

    double composedResult = (constraintEvaluation != 0) ? (0.8*constraintEvaluation + 0.2*game.result) : 0;
    torch::Tensor resultTensor = torch::full({1}, composedResult, torch::kFloat64);

    // cout << "composed result: " << composedResult << endl;
    // cout << "result tensor: " << resultTensor << endl;

    // if (gameIndex % 100000 == 0 )
    //     cout << "game: " << gameIndex << endl;

    constraintIndex++;
    datasetIndex++;
    if ( constraintIndex == constrains.size() )
        constraintIndex = 0;


    torch::Tensor constraintTensor = constrains[constraintIndex].toTensor();
    //cout << "constraint tensor: " << constraintTensor << endl;

    auto concatenated_tensor = torch::cat({boardTensor.view({1, -1}), constraintTensor.view({1, -1})}, 1);
    //cout << "concatenated tensor: " << concatenated_tensor << endl;

    return {concatenated_tensor, resultTensor};
}

// returns max number because dataset size is not known beforehand
torch::optional<size_t> ChessDataset::size() const {
    return datasetSize;
}

void ChessDataset::backToStart() {
    // cout << "End of the file reached" << endl;
    i.clear();               // clear fail and eof bits
    i.seekg(0);              // back to the start!

    gameIndex = 0;
    constraintIndex = 0;
    datasetIndex = 0;
}
