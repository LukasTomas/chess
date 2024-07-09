#include "Constraint.h"

using namespace std;

// CONSTRUCTORS ------------------------------------------------------------------------------------

Constraint::Constraint() :  Constraint(1, 2, 2, 2, 8) {}

Constraint::Constraint(int queens, int rooks, int bishops, int knights, int pawns) : _maxPossibleDifference(15 - (queens + rooks + bishops + knights + pawns)) {
    if ( queens > MAX_QUEENS || rooks > MAX_ROOKS || bishops > MAX_BISHOPS || knights > MAX_KNIGHTS || pawns > MAX_PAWNS )
        throw invalid_argument("Invalid constraint: too many pieces");

    constraints[pieceToIndexWithoutKing(Queen)] = queens;
    constraints[pieceToIndexWithoutKing(Rook)] = rooks;
    constraints[pieceToIndexWithoutKing(Bishop)] = bishops;
    constraints[pieceToIndexWithoutKing(Knight)] = knights;
    constraints[pieceToIndexWithoutKing(Pawn)] = pawns;

    if ( _maxPossibleDifference < 0 )
        throw invalid_argument("Invalid constraint: max possible difference is < 0");
}

Constraint & Constraint::operator=( const Constraint & other ) {
    constraints = other.constraints;
    _maxPossibleDifference = other._maxPossibleDifference;
    return *this;
}

// PUBLIC METHODS ----------------------------------------------------------------------------------
int Constraint::getConstrain(piece_t pieceType) const {
    return constraints[pieceToIndexWithoutKing(pieceType)];
}

// c * (1 - totalDifference/_maxPossibleDifference) + (1-c)
double Constraint::evaluate( array<int, MAX_ARRAY_SIZE_NO_KING> & pieceCounts ) const {
    double c = 1.0/16 - 0.01;

    double totalDifference = 0;

    for ( int i = 0; i < MAX_ARRAY_SIZE_NO_KING; i++ ) {
        int difference = pieceCounts[i] - constraints[i];
        if ( difference < 0 ) {
            // cout << "ERROR: piece count is less than constraint--------------------------------------------" << endl;
            return 0;
        }

        totalDifference += difference;
    }

    // print piece counts
    // cout << "\t\t piece counts: ";
    // for ( int i = 0; i < MAX_ARRAY_SIZE_NO_KING; i++ )
    //     cout << pieceCounts[i] << " ";
    // cout << endl;
    // cout << "\t\t total difference: " << totalDifference << endl;
    // cout << "\t\t max possible difference: " << _maxPossibleDifference << endl;
    // cout << "\t\t  /: " << totalDifference/_maxPossibleDifference << endl;
    // cout << "\t\t  1 - : " << (1 - totalDifference/_maxPossibleDifference) << endl;
    // cout << "\t\t  c * (1 - ): " << c * (1 - totalDifference/_maxPossibleDifference) << endl;
    // cout << "\t\t c: " << c << endl;
    // cout << "\t\t  (1-c): " << (1-c) << endl;
    // cout << "\t\t total: " << c * (1.0 - totalDifference/_maxPossibleDifference) + (1-c) << endl;

    return (1 - c) * (1.0 - totalDifference/_maxPossibleDifference) + c;
}

ostream & Constraint::save( ostream & os ) const {
    os.write(reinterpret_cast<const char*>(constraints.data()), sizeof(int) * MAX_ARRAY_SIZE_NO_KING);
    return os;
}

istream & Constraint::load( istream & is ) {
    is.read(reinterpret_cast<char*>(constraints.data()), sizeof(int) * MAX_ARRAY_SIZE_NO_KING);
    this->_maxPossibleDifference = 15 - accumulate(constraints.begin(), constraints.end(), 0);
    if ( _maxPossibleDifference < 0 )
        throw invalid_argument("Invalid constraint: max possible difference is < 0");
    return is;
}

string Constraint::toString() const {
    string s = "(";
    s += to_string(constraints[pieceToIndexWithoutKing(Queen)]) + ", ";
    s += to_string(constraints[pieceToIndexWithoutKing(Rook)]) + ", ";
    s += to_string(constraints[pieceToIndexWithoutKing(Bishop)]) + ", ";
    s += to_string(constraints[pieceToIndexWithoutKing(Knight)]) + ", ";
    s += to_string(constraints[pieceToIndexWithoutKing(Pawn)]) + ")";
    return s;
}

ostream & operator<<( ostream & os, const Constraint & c ) {
    os << c.toString();
    return os;
}

bool Constraint::operator==( const Constraint & other ) const {
    return constraints == other.constraints;
}
