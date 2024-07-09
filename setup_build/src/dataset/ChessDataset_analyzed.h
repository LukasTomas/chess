#pragma once

#include "StopIteration.h"

#include "BitboardAnalyzed.h"

#include "../evaluation/Constraints.h"
#include "../evaluation/Constraint.h"

#include "../terminal/Logging.h"

#include <fstream>
#include <vector>

#include <torch/torch.h>

using namespace std;


class ChessDataset_analyzed : public torch::data::Dataset<ChessDataset_analyzed> {
public:

    explicit ChessDataset_analyzed( const string & filePath );
    ChessDataset_analyzed(const ChessDataset_analyzed & dataset);

    torch::data::Example<> get(size_t index) override;
    torch::optional<size_t> size() const override;

private:

    vector<int> allowedConstraintIndexes = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    vector<BitboardAnalyzed> _bitboardsAnalyzed;

    //torch::Tensor states_, labels_;

    // sizes
    size_t _constrainSize = 0;
    size_t _datasetSize = 0;

};
