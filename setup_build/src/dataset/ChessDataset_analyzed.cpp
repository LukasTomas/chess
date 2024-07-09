#include "ChessDataset_analyzed.h"
#include "../engineCore/chessComponents/Bitboard.h"

#include <limits>


// CONSTRUCTORS ------------------------------------------------------------------------------------------------

ChessDataset_analyzed::ChessDataset_analyzed( const string & filePath ) {
    ifstream ifs(filePath, ios::binary);
    int i = 0;
    BitboardAnalyzed bitboardTmp;
    while ( bitboardTmp.load(ifs) ) {
        _bitboardsAnalyzed.push_back(bitboardTmp);
    }

    // // test --------------
    // for ( int j = 0; j < 2000000; j++ ) {
    //     BitboardAnalyzed & bitboardAnalyzed = _bitboardsAnalyzed[j];
    //     for ( int i = 0; i < constrains.size(); i++ ) {
    //         if ( bitboardAnalyzed._constraintSatisfiedProbab[i] > 0.9 && bitboardAnalyzed._constraintSatisfiedProbab[i] < 1)
    //             cout << i << ": " << bitboardAnalyzed._constraintSatisfiedProbab[i] << endl;
    //     }

    // }

    LOG_INFO << "Loaded " << _bitboardsAnalyzed.size() << " bitboards analyzed" << endl;

    // check constraints indexes
    LOG_INFO << "Allowed constraints: ";
    for ( int i = 0; i < allowedConstraintIndexes.size(); i++ ) {
        LOG_INFO << "\t" << constrains[allowedConstraintIndexes[i]] << endl;
        if ( allowedConstraintIndexes[i] >= constrains.size() ) {
            throw runtime_error("Constraint index out of range");
        }
    }
    _constrainSize = allowedConstraintIndexes.size();
    _datasetSize = _bitboardsAnalyzed.size() * _constrainSize;

    cout << "Bitboards:\t\t\t\t" << _bitboardsAnalyzed.size() << endl;
    cout << "Constrains:\t\t\t" << _constrainSize << endl;
    cout << "TOTAL DATASET SIZE:\t\t" << _datasetSize << endl;
    cout << "-----------------------------------------" << endl;

    double allocatedBytes = _bitboardsAnalyzed.size() * sizeof(BitboardAnalyzed);
    LOG_NOTICE << "Total ALLOCATED MEMORY: " << allocatedBytes/1'000'000 << "MB | " << allocatedBytes/1'000'000'000 << "GB" << endl << endl;
}

ChessDataset_analyzed::ChessDataset_analyzed(const ChessDataset_analyzed & dataset) {
    // cout << "copy constructor" << endl;
    _bitboardsAnalyzed = dataset._bitboardsAnalyzed;
    _constrainSize = dataset._constrainSize;
    _datasetSize = dataset._datasetSize;
}

// PUBLIC METHODS ----------------------------------------------------------------------------------------------

//42090555
torch::data::Example<> ChessDataset_analyzed::get(size_t index) {
    size_t bitboardIndex = index / _constrainSize;
    size_t constraintIndex = allowedConstraintIndexes[index % _constrainSize];

    const Bitboard & bitboard = _bitboardsAnalyzed[bitboardIndex]._bitboard;
    const double result = _bitboardsAnalyzed[bitboardIndex]._constraintSatisfiedProbab[constraintIndex];

    // DOESN'T share memory with board array
    torch::Tensor bitboardTensor = torch::zeros({6*2, 8, 8}, torch::kUInt8);
    bitboard.toTensor(bitboardTensor);

    torch::Tensor constraintAlgebraic = torch::zeros({5, 8, 8}, torch::kUInt8);
    constrains[constraintIndex].toAlgebraicTensor(constraintAlgebraic); //  ------------------------------------------- TODO MUZE B7T CHYBA PO VYSELEKTOVANI CONSTRIANS

    auto input = torch::cat({bitboardTensor, constraintAlgebraic}, 0);

    torch::Tensor resultTensor = torch::full({1}, result, torch::kFloat64);

    // cout << "board tensor: \n" << bitboardTensor << endl << endl;
    // cout << "constraint tensor: " << constraintTensor << endl << endl;
    // cout << "result tensor: " << resultTensor << endl << endl;

    return {input, resultTensor};
}


// returns max number because dataset size is not known beforehand
torch::optional<size_t> ChessDataset_analyzed::size() const {
    return _datasetSize;
    // return 50000;
}

// PRIVATE METHODS --------------------------------------------------------------------------------------------
