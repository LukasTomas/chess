#ifndef CONSTRAINTS_H
#define CONSTRAINTS_H

#include "Constraint.h"

#include <vector>
using namespace std;

const vector<Constraint> constrains = {
    // Constraint(1, 2, 2, 2, 8),
    // --------------------------- MISSING
    Constraint(1, 2, 2, 2, 7),  // 1 Pawn
    Constraint(1, 2, 2, 1, 8),  // 1 Knight
    Constraint(1, 2, 1, 2, 8),  // 1 Bishop
    Constraint(0, 2, 2, 2, 8),  // 1 Queen

    Constraint(1, 2, 2, 1, 7),  // 1 Pawn, 1 Knight
    Constraint(1, 2, 1, 2, 7),  // 1 Pawn, 1 Bishop
    Constraint(0, 2, 2, 2, 7),  // 1 Pawn, 1 Queen

    Constraint(1, 2, 1, 1, 7),  // 1 Pawn, 1 Knight, 1 Bishop
    Constraint(1, 2, 2, 0, 7),  // 1 Pawn, 2 Knights
    Constraint(1, 2, 0, 2, 7),  // 1 Pawn, 2 Bishops

    Constraint(1, 1, 2, 2, 8),  // 1 Rook
    Constraint(1, 2, 2, 2, 6),  // 2 Pawns

    // prumerne 19-21 tah
    Constraint(1, 2, 1, 1, 8),  // 1 Bishop, 1 Knight
    Constraint(1, 2, 2, 2, 5),  // 3 Pawns

    // na grafu skoro stejna krivka pro tyhle tri, ale az v 31 tahu
    Constraint(1, 2, 1, 1, 6),  // 1 Bishop, 1 Knight, 2 Pawns
    Constraint(1, 2, 2, 0, 6),  // 2 Knights, 2 Pawns
    Constraint(1, 2, 0, 2, 6)   // 2 Bishops, 2 Pawns
};

#endif // CONSTRAINTS_H
