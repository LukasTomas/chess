#pragma once

#include "parsing/ParsedGame.h"
#include "../evaluation/Constraints.h"
#include "../evaluation/Constraint.h"
#include "StopIteration.h"


#include <torch/torch.h>

#include <fstream>
using namespace std;


class ChessDataset : public torch::data::Dataset<ChessDataset> {
public:
    explicit ChessDataset( const string & filePath ) : path(filePath), i(filePath, ios::binary) {
        while( i >> game ) {
            gamesCount++;
            movesCount += game.moves.size();
        }

        datasetSize = movesCount * constrains.size();
        backToStart();

        cout << "games: " << gamesCount << endl;
        cout << "moves: " << movesCount << endl;
        cout << "constraints: " << constrains.size() << endl;
        cout << "total size: " << datasetSize << endl;
    }

    ~ChessDataset() {
        // cout << "destoried" << endl;
        i.close();
    }

    ChessDataset(const ChessDataset & dataset) : path(dataset.path), i(dataset.path, ios::binary) {
        // cout << "copy constructor" << endl;

        gamesCount = dataset.gamesCount;
        movesCount = dataset.movesCount;
        datasetSize = dataset.datasetSize;

        // cout << "others index " << dataset.datasetIndex << endl;
        if ( dataset.datasetIndex != 0 )
            throw logic_error("copy constructor with dataset index != 0");

        // cout << endl;
    }


    torch::data::Example<> get(size_t index) override;

    torch::optional<size_t> size() const override;

private:
    string path;
    ifstream i;
    ParsedGame game;

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

    void backToStart();

};
