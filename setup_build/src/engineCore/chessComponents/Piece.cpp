#include <cstdint>

#include "Piece.h"

Piece::Piece() :colorPiece(0) {}

Piece::Piece( int index, side_t side ) : Piece((index + 1) | side) {
    // assuming the first value in enum is None = 0
    CHECK(PieceType::None == 0);
    CHECK(index >= 0 && index < PieceType::Max - 1);
}

Piece::Piece( uint8_t colorPiece ) : colorPiece(colorPiece) {
    CHECK(
        (getSide() == White || getSide() == Black) && (getPieceType < PieceType::Max)
    );
}

Piece & Piece::operator=( const Piece & p) {
    // Guard self assignment
    if (this == &p)
        return *this;

    this->colorPiece = p.colorPiece;
    return *this;
}

piece_t Piece::getPieceType() const {
    return colorPiece & 0x7;
}

side_t Piece::getSide() const {
    return colorPiece & 0x18;
}

string Piece::getPieceName() const {
    return PIECE_NAMES.at(getPieceType());
}

string Piece::getSideName() const {
    return SIDE_NAMES.at(getSide());
}

string Piece::toStr() const {
    return getSideName() + " " + getPieceName();
}

int Piece::toIndex() const {
    return pieceToIndex(getPieceType());
}

int Piece::toIndexWithoutKing() const {
    return pieceToIndexWithoutKing(getPieceType());
}

bool Piece::isNone() const {
    return colorPiece == 0;
}

bool Piece::isWhite() const {
    return getSide() == White;
}

bool Piece::isBlack() const {
    return getSide() == Black;
}

bool Piece::sameSide( const Piece & p ) const {
    return getSide() == p.getSide();
}

bool Piece::operator==( const Piece & p ) const {
    return colorPiece == p.colorPiece;
}

bool Piece::operator!=( const Piece & p ) const {
    return !(*this == p);
}

