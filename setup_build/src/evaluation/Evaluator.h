#ifndef __EVALUATOR_H__
#define __EVALUATOR_H__

#include "../engineCore/ChessDefinitions.h"
#include "../engineCore/chessComponents/Piece.h"
#include "../engineCore/chessComponents/Piece.h"
#include "../engineCore/chessComponents/Board.h"

#include "Constraint.h"

#include "../neural/ConvNet.h"

#include <vector>
#include <array>
#include <iostream>

#include <torch/torch.h>

using namespace std;

class Evaluator
{
public:
    static const int PAWN_VALUE = 100;
    static const int KNIGHT_VALUE = 300;
    static const int BISHOP_VALUE = 300;
    static const int ROOK_VALUE = 500;
    static const int QUEEN_VALUE = 900;

    static const int VALUES[5];

    // only for white
    // -1, because all pieces without king
    const array<uint, MAX_ARRAY_SIZE_NO_KING> constrains = {0};

    Evaluator(const Board &board, const Constraint & constraint, double constraintWeight = 0.8)
    : board(board), constraint(constraint), constraintWeight(constraintWeight), materialWeight(1 - constraintWeight),
      net(64, 5) {
        if (constraintWeight < 0 || constraintWeight > 1)
            throw logic_error("constraintWeight must be between 0 and 1");

        torch::load(net, "models/31.1./best_model.pt");
        // for (const auto& p : net->named_parameters())
        //     std::cout << p.key() << ": " << p.value() << std::endl;
    }


    int CountMaterial(side_t side) const;

    int EvaluateMaterial(side_t sideToMove) const;
    int EvaluateConstrains() const;

    double Evaluate(side_t sideToMove);

    bool ConstraintSatisfied() const;

private:
    const Board &board;
    const Constraint & constraint;

    const double constraintWeight, materialWeight;

    ConvNet net;


    double normalize(int value, int min, int max) const;
    double predict();

};

#endif //__EVALUATOR_H__
