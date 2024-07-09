#include "BitboardAnalyzed.h"
#include "../evaluation/Constraints.h"

using namespace std;

istream & BitboardAnalyzed::load( std::istream & is ) {
    _bitboard.load(is);

    // load constraint satisfaction probablity
    for ( int i = 0; i < constrains.size(); i++ )
        is.read(reinterpret_cast<char*>(&_constraintSatisfiedProbab[i]), sizeof(_constraintSatisfiedProbab[i]));

    return is;
}

ostream & BitboardAnalyzed::save( ostream & os ) const {
    _bitboard.save(os);
    
    for ( int i = 0; i < constrains.size(); i++ )
        os.write(reinterpret_cast<const char*>(&_constraintSatisfiedProbab[i]), sizeof(_constraintSatisfiedProbab[i]));

    return os;
}
