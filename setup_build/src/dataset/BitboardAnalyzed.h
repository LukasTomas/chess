#pragma once

#include "../engineCore/chessComponents/Bitboard.h"
#include "../evaluation/Constraints.h"

#include <iostream>
#include <vector>

class BitboardAnalyzed {
public:

    Bitboard _bitboard;
    std::vector<double> _constraintSatisfiedProbab;

    BitboardAnalyzed() {
        _constraintSatisfiedProbab.resize(constrains.size(), 0);
    }

    std::istream & load( std::istream & is );
    std::ostream & save( std::ostream & os ) const;


private:

};
