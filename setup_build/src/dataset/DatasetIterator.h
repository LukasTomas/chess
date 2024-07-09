#pragma once

//#include "parsing/IAdditionalGameData.h"
#include "parsing/ParsedGame.h"
#include "../evaluation/Constraints.h"

#include <fstream>
#include <vector>

/**
 * @brief
 *
 * @tparam T Additional data for each ParsedGame like constraint results, . . .
 */
template <class T>
class DatasetIterator {
public:
    //static_assert(std::is_base_of<IAdditionalGameData, T>, "Template T must inherit interface IAdditionalGameData")

    DatasetIterator( const std::string & filepath );
    ~DatasetIterator();

    ParsedGame * LoadNext();
    void Reset();
    bool IsEnd() const;

    bool AreConstraints() const;
    const vector<Constraint> & GetConstraints() const;

    inline ParsedGame * CurrentGame() const {
        if ( gamesCount == 0 )
            return nullptr;
        return currentGame;
    }
    inline T * CurrentAdditionalData() const {
        if ( gamesCount == 0 )
            return nullptr;
        return currentAdditionalData;
    }

private:

    std::streampos _firstGameStreamPos;

    bool _areConstraints = false;
    vector<Constraint> _constraints;

    unsigned int gamesCount = 0;
    bool _isThereFirstGame = false;

    std::ifstream input;
    ParsedGame * currentGame = new ParsedGame();
    T * currentAdditionalData = new T();
};
