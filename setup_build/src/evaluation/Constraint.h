#pragma once

#include "../engineCore/ChessDefinitions.h"

#include <array>
#include <iostream>

#include "../IS_LIB_TORCH.h"
#ifdef IS_LIB_TORCH
    #include <torch/torch.h>
#endif

class Constraint {
public:

    const static int MAX_QUEENS = 1;
    const static int MAX_ROOKS = 2;
    const static int MAX_BISHOPS = 2;
    const static int MAX_KNIGHTS = 2;
    const static int MAX_PAWNS = 8;

    array<int, MAX_ARRAY_SIZE_NO_KING> constraints = {0};


    Constraint();
    Constraint(int queens, int rooks, int bishops, int knights, int pawns);
    Constraint & operator=( const Constraint & other );

    int getConstrain(piece_t pieceType) const;

    double evaluate( array<int, MAX_ARRAY_SIZE_NO_KING> & pieceCounts ) const;

    #ifdef IS_LIB_TORCH
        inline torch::Tensor toTensor() const {
            // Cast the const void* to void* to resolve the issue
            void* dataPtr = const_cast<int*>(constraints.data());
            return torch::from_blob(dataPtr, {MAX_ARRAY_SIZE_NO_KING}, torch::kInt32).clone();
        }

        inline void toAlgebraicTensor( torch::Tensor tensor ) const {
            // should have 5 x 8 x 8 shape
            for ( int i = 0; i < MAX_ARRAY_SIZE_NO_KING; i++ ) {
                int constraintValue = constraints[i];
                torch::Tensor plane = torch::full({8, 8}, constraintValue);
                tensor[i] = plane;
            }
        }
    #endif

    ostream & save( ostream & os ) const;
    istream & load( istream & is );

    std::string toString() const;
    friend ostream & operator<<( ostream & os, const Constraint & c );

    bool operator==( const Constraint & other ) const;

private:

    int _maxPossibleDifference;

};
