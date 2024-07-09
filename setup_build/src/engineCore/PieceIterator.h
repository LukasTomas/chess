#ifndef __PIECEITERATOR_H__
#define __PIECEITERATOR_H__

#include <map>
#include <string>
#include <stdexcept>

#include "ChessDefinitions.h"

using namespace std;


class PieceIterator {
public:

    int current;

    PieceIterator( bool includeNone = false ) : includeNone(includeNone) {
        this->current = includeNone ? PieceType::None : PieceType::King;
    }

    bool next() {
        current++;
        return current <= max;
    }

private:

    const bool includeNone;
    const int max = PieceType::Queen;

};


#endif  //__PIECEITERATOR_H__
