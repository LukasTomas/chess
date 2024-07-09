#pragma once

#include "Coors.h"
#include <cmath>
using namespace std;

class Vector {
public:

    Vector( int x, int y ) : x(x), y(y) {}

    Vector( const coors & from ) {
        x = from.x;
        y = from.y;
    }

    Vector( const coors & from, const coors & to ) {
        x = to.x - from.x;
        y = to.y - from.y;
    }

    Vector( const coors & from, const coors & to, bool normalize ) : Vector(from, to){
        if ( normalize ) {
            double length = Length();
            x /= length;
            y /= length;
        }
    }

    double Length() const {
        return sqrt(x*x + y*y);
    }

    coors Get() const {
        return {x, y};
    }

    double Angle() const {
        double angle = atan2(y, x) * 180 / M_PI;
        if (angle < 0) {
            angle += 360;
        }
        return angle;
    }

    bool operator==( const Vector & other ) const {
        return x == other.x && y == other.y;
    }

    bool operator!=( const Vector & other ) const {
        return !(*this == other);
    }

    bool operator<( const Vector & other ) const {
        if ( x == other.x )
            return y < other.y;

        return x < other.x;
    }

    bool operator>( const Vector & other ) const {
        if ( x == other.x )
            return y > other.y;

        return x > other.x;
    }

    int x, y;

};
