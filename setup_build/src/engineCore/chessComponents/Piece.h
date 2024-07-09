#ifndef __PIECE_H__
#define __PIECE_H__

#include <cstdint>
#include <map>

#include "../ChessDefinitions.h"

using namespace std;

class Piece {
public:

    uint8_t colorPiece;

    Piece();
    Piece( uint8_t colorPiece );
    Piece( int index, side_t side );

    Piece & operator=( const Piece & p );

    piece_t getPieceType() const;
    side_t getSide() const;

    string getPieceName() const;
    string getSideName() const;
    string toStr() const;

    int toIndex() const;
    int toIndexWithoutKing() const;

    bool isNone() const;

    bool isWhite() const;
    bool isBlack() const;

    bool sameSide( const Piece & p ) const;
    bool operator==( const Piece & p ) const;
    bool operator!=( const Piece & p ) const;

    friend ostream & operator<<( ostream & o, const Piece & piece ) {
        o << piece.toStr();
        return o;
    }

private:

};


#endif  //__PIECE_H__
