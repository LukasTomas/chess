#ifndef __BOARD_H__
#define __BOARD_H__

#include "../ChessDefinitions.h"
#include "Bitboard.h"
#include "Piece.h"

#include "../../IS_LIB_TORCH.h"
#ifdef IS_LIB_TORCH
    #include <torch/torch.h>
#endif

#include <array>
#include <cstdint>

using namespace std;

/*
     a  | b  | c  | d  | e  | f  | g  | h
8    56 | 57 | 58 | 59 | 60 | 61 | 62 | 63
7    48 | 49 | 50 | 51 | 52 | 53 | 54 | 55
6    40 | 41 | 42 | 43 | 44 | 45 | 46 | 47
5    32 | 33 | 34 | 35 | 36 | 37 | 38 | 39
4    24 | 25 | 26 | 27 | 28 | 29 | 30 | 31
3    16 | 17 | 18 | 19 | 20 | 21 | 22 | 23
2    8  | 9  | 10 | 11 | 12 | 13 | 14 | 15
1    0  | 1  | 2  | 3  | 4  | 5  | 6  | 7
 */

class Board
{
public:
    enum SquareColor
    {
        Light,
        Dark
    };

    const size_t n_squares;

    Board(size_t n_squares = chessboardSize::SQUARES);
    Board(const Board &t);
    ~Board();

    Board &operator=(const Board &board);

    void toBitboard( uint64_t bitboard[] ) const;
    void setFromBitboard( Bitboard & bitboard );

    bool operator==(const Board &other) const;
    bool operator!=(const Board &other) const;

    Piece operator[](size_t index) const;
    Piece &operator[](size_t index);

    int getSquareColor(size_t index) const;

    #ifdef IS_LIB_TORCH
        torch::Tensor get2dTensor() const;
        void toBitboardsTensor( torch::Tensor & tensor ) const;
    #endif

    string toString() const;
    friend ostream & operator<<( ostream & o, const Board & game );


private:
    Piece * board;
};

#endif
