#pragma once

#include <iostream>
using namespace std;

struct coors {
    int x;
    int y;

    void operator= (const coors & coors ) {
        x = coors.x;
        y = coors.y;
    }

    bool operator==( const coors & other ) const {
        return ( x == other.x && y == other.y );
    }

    bool operator<( const coors & other ) const {
        if ( x == other.x )
            return y < other.y;
        else
            return x < other.x;
    }

    bool operator>( const coors & other ) const {
        if ( x == other.x )
            return y > other.y;
        else
            return x > other.x;
    }

    friend ostream & operator<<( ostream & os, const coors & coors ) {
        os << "(" << coors.x << ", " << coors.y << ")";
        return os;
    }

};
