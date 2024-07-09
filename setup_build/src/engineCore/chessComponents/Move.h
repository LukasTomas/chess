#ifndef __MOVE_H__
#define __MOVE_H__

#include "../ChessDefinitions.h"

#include <cstdint>
#include <iostream>

struct Move {

    Move () : Move(-1, -1, PieceType::None, false, false, false) { }

    Move( uint8_t from, uint8_t to, piece_t promotion = PieceType::None, bool isTake = false, bool isKingCastle = false, bool isQueenCastle = false )
     :  from(from),
        to(to),
        promotion(promotion),
        isTake(isTake),
        isKingCastle(isKingCastle),
        isQueenCastle(isQueenCastle) { }

    Move( const Move & m )
     :  from(m.from),
        to(m.to),
        promotion(m.promotion),
        isTake(m.isTake),
        isKingCastle(m.isKingCastle),
        isQueenCastle(m.isQueenCastle) { }

    Move & operator=( const Move & m ) {
        if (this != &m) {
            from = m.from;
            to = m.to;
            promotion = m.promotion;
            isTake = m.isTake;
            isKingCastle = m.isKingCastle;
            isQueenCastle = m.isQueenCastle;
        }

        return *this;
    }

    uint8_t from, to;
    piece_t promotion;
    bool isTake;
    bool isKingCastle;
    bool isQueenCastle;

    bool isPromotion() const {
        return promotion != PieceType::None;
    }

    bool isCastle() const {
        return isKingCastle || isQueenCastle;
    }


    bool operator==( const Move & m ) const {
        return from == m.from && to == m.to && promotion == m.promotion && isTake == m.isTake
            && isKingCastle == m.isKingCastle && isQueenCastle == m.isQueenCastle;
    }

    bool operator<( const Move & m ) const {
        if ( from == m.from ) {
            if (to == m.to)
                return promotion < m.promotion;

            return to < m.to;
        }

        return from < m.from;
    }

    bool operator>( const Move & m ) const {
        if ( from == m.from ) {
            if (to == m.to)
                return promotion > m.promotion;

            return to > m.to;
        }

        return from > m.from;
    }

    ostream & save( ostream & o ) const {
        o.write(reinterpret_cast<const char*>(&from), sizeof(from));
        o.write(reinterpret_cast<const char*>(&to), sizeof(to));
        o.write(reinterpret_cast<const char*>(&promotion), sizeof(promotion));
        o.write(reinterpret_cast<const char*>(&isTake), sizeof(isTake));
        o.write(reinterpret_cast<const char*>(&isKingCastle), sizeof(isKingCastle));
        o.write(reinterpret_cast<const char*>(&isQueenCastle), sizeof(isQueenCastle));

        return o;
    }

    istream & load( istream & i ) {
        i.read(reinterpret_cast<char*>(&from), sizeof(from));
        i.read(reinterpret_cast<char*>(&to), sizeof(to));
        i.read(reinterpret_cast<char*>(&promotion), sizeof(promotion));
        i.read(reinterpret_cast<char*>(&isTake), sizeof(isTake));
        i.read(reinterpret_cast<char*>(&isKingCastle), sizeof(isKingCastle));
        i.read(reinterpret_cast<char*>(&isQueenCastle), sizeof(isQueenCastle));

        return i;
    }

    friend ostream & operator<<( ostream & o, const Move & m ) {
        /*o << (int)move.from << " - " << (int)move.to << " | " << " is promotion: " << move.isPromotion() << ", is take: " << move.isTake;
        o << ", is king castle: " << move.isKingCastle << ", is queen castle: " << move.isQueenCastle;*/

        o.write(reinterpret_cast<const char*>(&m.from), sizeof(m.from));
        o.write(reinterpret_cast<const char*>(&m.to), sizeof(m.to));
        o.write(reinterpret_cast<const char*>(&m.promotion), sizeof(m.promotion));
        o.write(reinterpret_cast<const char*>(&m.isTake), sizeof(m.isTake));
        o.write(reinterpret_cast<const char*>(&m.isKingCastle), sizeof(m.isKingCastle));
        o.write(reinterpret_cast<const char*>(&m.isQueenCastle), sizeof(m.isQueenCastle));

        /*o << move.from;
        o << move.to;
        o << move.promotion;
        o << move.isTake;
        o << move.isKingCastle;
        o << move.isQueenCastle;*/
        return o;
    }

    friend istream & operator>>( istream & i, Move & m ) {
        i.read(reinterpret_cast<char*>(&m.from), sizeof(m.from));
        i.read(reinterpret_cast<char*>(&m.to), sizeof(m.to));
        i.read(reinterpret_cast<char*>(&m.promotion), sizeof(m.promotion));
        i.read(reinterpret_cast<char*>(&m.isTake), sizeof(m.isTake));
        i.read(reinterpret_cast<char*>(&m.isKingCastle), sizeof(m.isKingCastle));
        i.read(reinterpret_cast<char*>(&m.isQueenCastle), sizeof(m.isQueenCastle));

        /*i >> move.from;
        i >> move.to;
        i >> move.promotion;
        i >> move.isTake;
        i >> move.isKingCastle;
        i >> move.isQueenCastle;*/
        return i;
    }

    void print() const {
        cout << (int)from << " - " << (int)to << endl;
        cout << "promotion " << PIECE_NAMES.at(promotion) << endl;
        cout << "is take" << (isTake ? " true" : " false")
        << ", is king castle " << (isKingCastle ? " true" : " false")
        << ", is queen castle " << (isQueenCastle ? " true" : " false") << endl;
    }

};


#endif  //__MOVE_H__
