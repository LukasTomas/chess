#pragma once

#include "Vector.h"

#include <string>
using namespace std;

class LineQuadrant {
public:

/**
 * @brief Constructs a LineQuadrant object with the given parameters in PI radians.
 *
 * @param zero                  0 radians
 * @param zeroToOneQuarter      (0, 1/4 PI) in radians
 * @param oneQuarter            1/4 PI radians
 * @param oneQuarterToOneHalf   (1/4 PI, 1/2 PI) in radians.
 * @param oneHalf               1/2 PI radians
 */
    LineQuadrant( const string & zero, const string & zeroToOneQuarter, const string & oneQuarter, const string & oneQuarterToOneHalf, const string & oneHalf );
    void operator=( const LineQuadrant & other );

    const string & GetByAngle( double angle ) const;
    const string & GetByVector( const Vector & vector ) const;

    LineQuadrant InvertByX() const;
    LineQuadrant InvertByY() const;

    void Print() const;

private:

    string _zero, _oneHalf, _oneQuarter, _zeroToOneQuarter, _oneQuarterToOneHalf;

};
