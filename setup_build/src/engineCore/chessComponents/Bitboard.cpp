#include "Bitboard.h"

using namespace std;

istream & Bitboard::load( istream & is ) {
    for ( int i = 0; i < SIZE; i++ )
        is.read(reinterpret_cast<char*>(&binPlanes[i]), sizeof(binPlanes[i]));
    return is;
}

ostream & Bitboard::save( ostream & os ) const {
    for ( int i = 0; i < SIZE; i++ )
        os.write(reinterpret_cast<const char*>(&binPlanes[i]), sizeof(binPlanes[i]))    ;
    return os;
}

string Bitboard::toString() const {
    string str = "";
    for ( int i = 0; i < SIZE; i++ ) {
        for ( int j = 0; j < 64; j++ ) {
            if ( j % 8 == 0 )
                str += "\n";
            str += (binPlanes[i] & (1ULL << j) ? "1" : "0");
        }
        str += "\n";
    }
    return str;
}

#ifdef IS_LIB_TORCH
    void Bitboard::toTensor( torch::Tensor & tensor ) const {
        // if ( tensor.sizes() != torch::IntArrayRef({12, 8, 8}) )
        //     throw invalid_argument("Bitboard::toTensor: tensor has invalid shape");

        for ( int i = 0; i < SIZE; i++ ) {
            for ( int j = 0; j < 64; j++ ) {
                int row = j / 8;
                int col = j % 8;
                tensor[i][row][col] = (binPlanes[i] & (1ULL << j) ? 1 : 0);
            }
        }
    }
#endif
