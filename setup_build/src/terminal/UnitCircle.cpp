#include "UnitCircle.h"

#include <cmath>

// CONSTRUCTORS ------------------------------------------------------------------
UnitCircle::UnitCircle( const map<int, string> & circle ) : _circle(circle) {
    for ( auto & [key, value] : _circle ) {
        if ( key < 0 || key > 360 ) {
            throw "UnitCircle: key must be between 0 and 360";
        }
    }
}

UnitCircle::UnitCircle( const map<Vector, string> & circle ) {
    for ( auto & [key, value] : circle ) {
        _circle[key.Angle()] = value;
    }
}

// PUBLIC METHODS ----------------------------------------------------------------
void UnitCircle::SetRotation( int rotation ) {
    this->rotation = rotation;
}

void UnitCircle::ResetRotation( int rotation ) {
    this->rotation = 0;
}

int UnitCircle::GetRotation() const {
    return rotation;
}

void UnitCircle::Invert( char alongAxis ) {
    if ( alongAxis == 'x')
        invertX = true;
    else if ( alongAxis == 'y' )
        invertY = true;
    else
        throw "UnitCircle: alongAxis must be 'x' or 'y'";
}

void UnitCircle::ResetInvert() {
    invertX = false;
    invertY = false;
}

string UnitCircle::GetByAngle ( int angle ) const {
    // first do invert
    if ( invertX && invertY )
        angle += 180;
    else if ( invertX )
        angle = 180 - angle;
    else if ( invertY )
        angle = 360 - angle;

    if ( angle < 0 )
        angle += 360;

    int newAngle = angle + rotation;
    if ( newAngle < 0 )
        newAngle += 360;
    else if ( newAngle >= 360 )
        newAngle %= 360;

    return _circle.at(newAngle);
}

string UnitCircle::GetByVector( const Vector & vector ) const {
    return GetByAngle(vector.Angle());
}

// PRIVATE METHODS ---------------------------------------------------------------
int UnitCircle::Quadrant( int angle ) {
    return ceil(angle/90);
}

