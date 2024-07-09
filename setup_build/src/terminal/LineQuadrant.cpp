#include "LineQuadrant.h"
#include "DrawingChars.h"

LineQuadrant::LineQuadrant( const string & zero, const string & zeroToOneQuarter, const string & oneQuarter, const string & oneQuarterToOneHalf, const string & oneHalf ) {
    _zero = zero;
    _zeroToOneQuarter = zeroToOneQuarter;
    _oneQuarter = oneQuarter;
    _oneQuarterToOneHalf = oneQuarterToOneHalf;
    _oneHalf = oneHalf;
}

void LineQuadrant::operator=( const LineQuadrant & other ) {
    _zero = other._zero;
    _zeroToOneQuarter = other._zeroToOneQuarter;
    _oneQuarter = other._oneQuarter;
    _oneQuarterToOneHalf = other._oneQuarterToOneHalf;
    _oneHalf = other._oneHalf;
}

// PUBLIC ----------------------------------------------------------------------------------------

LineQuadrant LineQuadrant::InvertByX() const {
    return LineQuadrant();
}

LineQuadrant LineQuadrant::InvertByY() const {
    return LineQuadrant(_zero, _oneQuarterToOneHalf, _oneQuarter, _zeroToOneQuarter, _oneHalf)
}

void LineQuadrant::Print() const {
    const int length = 10;

    cout << string(length, '-') << " - " << _zero << endl;

    for ( int i = 0; i < length; i++ ) {
        cout << "|";

        int minusSpaces = 0;
        if ( i == length/2 ) {
            cout << " " << _zeroToOneQuarter;
            minusSpaces = 1 + _zeroToOneQuarter.length();
        }

        string spaces(i - minusSpaces, ' ');
        cout << spaces << boxDrwaing::LIGHT_DIAGONAL_UPP_LFT_TO_LOW_RIGHT;

        if ( i == length/2 )
            cout << "   " << _oneQuarterToOneHalf;

        cout << endl;
    }

    cout << "\n|" + string(length+1, ' ') << boxDrwaing::LIGHT_DIAGONAL_UPP_LFT_TO_LOW_RIGHT << endl;
    cout << _oneHalf + string(length+2, ' ') + _oneQuarter << endl;
}

