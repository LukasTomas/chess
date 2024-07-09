#pragma once

#include "Vector.h"

#include <map>
using namespace std;

class UnitCircle {
public:

    UnitCircle( const map<int, string> & circle );
    UnitCircle( const map<Vector, string> & circle );

    void SetRotation( int rotation );
    void ResetRotation( int rotation );
    int GetRotation() const;

    void Invert( char alongAxis );
    void ResetInvert();

    string GetByAngle( int angle ) const;
    string GetByVector( const Vector & vector ) const;

private:

    map<int, string> _circle;

    int rotation = 0;
    bool invertX = false;
    bool invertY = false;

    static int Quadrant( int angle );

};
