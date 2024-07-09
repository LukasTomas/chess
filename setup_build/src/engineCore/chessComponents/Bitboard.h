#pragma once

#include <cstdint>
#include <iostream>

#include "../../IS_LIB_TORCH.h"
#ifdef IS_LIB_TORCH
    #include <torch/torch.h>
#endif


struct Bitboard {

    static const int SIZE = 12;

    int64_t binPlanes[SIZE];

    bool operator==(const Bitboard &other) const {
        for ( int i = 0; i < SIZE; i++ )
            if ( binPlanes[i] != other.binPlanes[i] )
                return false;
        return true;
    }

    bool operator<(const Bitboard &other) const {
        for ( int i = 0; i < SIZE; i++ )
            if ( binPlanes[i] < other.binPlanes[i] )
                return true;
            else if ( binPlanes[i] > other.binPlanes[i] )
                return false;
        return false;
    }

    bool operator>(const Bitboard &other) const {
        for ( int i = 0; i < SIZE; i++ )
            if ( binPlanes[i] > other.binPlanes[i] )
                return true;
            else if ( binPlanes[i] < other.binPlanes[i] )
                return false;
        return false;
    }

    std::istream & load( std::istream & is );
    std::ostream & save( std::ostream & os ) const;

    std::string toString() const;

    #ifdef IS_LIB_TORCH
        void toTensor( torch::Tensor & tensor ) const;
    #endif


};
