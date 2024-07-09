#pragma once

#include "StopIteration.h"
#include "DatasetIterator.h"
#include "parsing/ParsedGame.h"
#include "parsing/ConstraintGameData.h"

#include "../evaluation/Constraints.h"
#include "../evaluation/Constraint.h"

#include <fstream>
#include <vector>

#include <torch/torch.h>

using namespace std;


class ChessDataset_2 : public torch::data::Dataset<ChessDataset_2> {
public:

    explicit ChessDataset_2( const string & filePath );
    ChessDataset_2(const ChessDataset_2 & dataset);

    torch::data::Example<> get(size_t index) override;
    torch::optional<size_t> size() const override;

private:
    static const int ELO_THRESHOLD = 1000;
    static const vector<string> NOT_ALLOWED_TIMECONTROL;

    string path;
    DatasetIterator<ConstraintGameData> iterator;

    //torch::Tensor states_, labels_;
    // indexes
    int gameIndex = 0;
    int constraintIndex = 0;
    int datasetIndex = 0;

    // sizes
    size_t gamesCount = 0;
    size_t movesCount = 0;
    // total size
    size_t datasetSize = 0;

    double getResult() const;
    void backToStart();
    void getConstraintResults();
    ParsedGame * loadNextGame();

};