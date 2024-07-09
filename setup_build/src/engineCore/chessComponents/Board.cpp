#include "Board.h"

#include "../ChessDefinitions.h"

#include <stdexcept>
#include <sstream>

Board::Board(size_t n_squares) : n_squares(n_squares)
{
    this->board = new Piece[n_squares]();
}

Board::Board(const Board &board) : n_squares(board.n_squares)
{
    this->board = new Piece[board.n_squares];
    for (size_t i = 0; i < board.n_squares; i++)
        this->board[i] = board.board[i];
}

Board::~Board()
{
    delete[] board;
}

Board & Board::operator=(const Board &other)
{
    if (this->n_squares != other.n_squares)
        throw invalid_argument("boards of different sizes");

    for (size_t i = 0; i < this->n_squares; i++)
        this->board[i] = other.board[i];

    return *this;
}

void Board::toBitboard( uint64_t bitboard[] ) const {
    const size_t bitboardDim = 12;
    for ( size_t i = 0; i < bitboardDim; i++ )
        bitboard[i] = 0;

    for ( size_t i = 0; i < n_squares; i++ ) {
        Piece piece = board[i];
        if ( piece == PieceType::None )
            continue;

        int pieceIndex = piece.toIndex();
        if ( piece.isBlack() )
            pieceIndex += 6;

        bitboard[pieceIndex] |= (1ULL << i);
    }
}

void Board::setFromBitboard( Bitboard & bitboard ) {
    // first clean
    for ( int i = 0; i < 64; i++ )
        this->board[i] = PieceType::None;


    for ( int i = 0; i < 12; i++ ) {
        side_t side = (i > 5) ? Side::Black : Side::White;
        piece_t pieceType = pieceFromIndex(i % 6);
        cout << "pieceType: " << PIECE_NAMES.at(pieceType) << ", " << SIDE_NAMES.at(side) << endl;

        for ( int j = 0; j < 64; j++ ) {
            char c = (bitboard.binPlanes[i] & (1ULL << j) ? '1' : '0');
            if ( c == '0' )
                continue;

            this->board[j] = Piece( pieceType | side);
        }
    }
}

bool Board::operator==(const Board &other) const
{
    if (this->n_squares != other.n_squares)
        return false;

    for (int i = 0; i < n_squares; i++)
    {
        if (other.board[i] != this->board[i])
            return false;
    }

    return true;
}

bool Board::operator!=(const Board &other) const
{
    return !(*this == other);
}


Piece Board::operator[](size_t index) const
{
    if (index >= n_squares)
        throw out_of_range("index out of range: " + index);

    return board[index];
}

Piece &Board::operator[](size_t index)
{
    if (index >= n_squares)
        throw out_of_range("index out of range: " + index);

    return board[index];
}

int Board::getSquareColor(size_t index) const
{
    if (index >= n_squares)
        throw out_of_range("index out of range: " + index);

    int rank = index / chessboardSize::LINE_SQUARES; // rows
    int file = index % chessboardSize::LINE_SQUARES; // columns

    bool isLight = (file + rank) % 2 != 0;
    return isLight ? Light : Dark;
}

#ifdef IS_LIB_TORCH
    torch::Tensor Board::get2dTensor() const {
        auto options = torch::TensorOptions().dtype(torch::kUInt8);
        torch::Tensor tensor = torch::from_blob(board, {(long) n_squares}, options);
        return tensor.reshape({8, 8});
    }

    void Board::toBitboardsTensor( torch::Tensor & tensor ) const {
        int pieceTypes = 6;
        //torch::Tensor tensor = torch::zeros({pieceTypes*2, 8, 8}, torch::kUInt8);

        for (int i = 0; i < n_squares; i++) {
            Piece piece = board[i];
            if (piece == PieceType::None)
                continue;

            int pieceIndex = piece.toIndex();
            if ( piece.isBlack() )
                pieceIndex += pieceTypes;
            /* Indexes:
             *      index of piece type (0-5) + index of side (0-1)
             *      row index (0-7)
             *      column index (0-7)
            */
            tensor[pieceIndex][i / 8][i % 8] = 1;
        }

    }

#endif

string Board::toString() const {
    stringstream ss;

    for ( int i = 0; i < n_squares; i++ ) {
        if ( i % 8 == 0 )
            ss << endl;

        ss << ((int)board[i].colorPiece) << " ";
    }

    return ss.str();
}

ostream & operator<<( ostream & o, const Board & board ) {
    o << board.toString();
    return o;
}
