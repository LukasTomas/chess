#include "Evaluator.h"

#include "../engineCore/chessComponents/Bitboard.h"

#include <numeric>
#include <cstdlib>

const int Evaluator::VALUES[5] = {PAWN_VALUE, KNIGHT_VALUE, BISHOP_VALUE, ROOK_VALUE, QUEEN_VALUE};

double Evaluator::Evaluate(side_t sideToMove) {
    return predict();

    // ----------------- OLD EVALUATION -----------------

    int materialScore = EvaluateMaterial(sideToMove);
    double normalizedMaterialScore = normalize(materialScore, PAWN_VALUE, QUEEN_VALUE);
    double normalizedConstrainsScore = predict();

    double evaluation = constraintWeight * normalizedConstrainsScore + materialWeight * normalizedMaterialScore;
    return evaluation;
}

int Evaluator::EvaluateMaterial(side_t sideToMove) const
{
    int whiteMaterial = CountMaterial(Side::White);
    int blackMaterial = CountMaterial(Side::Black);

    int evaluation = whiteMaterial - blackMaterial;

    int perspective = sideToMove == Side::White ? 1 : -1;
    return perspective * evaluation;
}

int Evaluator::CountMaterial(side_t side) const
{
    int material = 0;

    for (int i = 0; i < board.n_squares; i++)
    {
        Piece square = board[i];

        piece_t piece = square.getPieceType();
        if (piece == PieceType::None)
            continue;

        if (!(side == square.getSide()))
            continue;

        if (piece == PieceType::King)
            continue;

        material += VALUES[square.toIndexWithoutKing()];
    }

    return material;
}

int Evaluator::EvaluateConstrains() const
{
    array<int, MAX_ARRAY_SIZE_NO_KING> piecesCount = {0};

    for (int i = 0; i < board.n_squares; i++)
    {
        Piece square = board[i];

        piece_t piece = square.getPieceType();
        if (piece == PieceType::None)
            continue;

        if (piece == PieceType::King)
            continue;

        if (square.isBlack())
            continue;

        piecesCount[square.toIndexWithoutKing()]++;
    }

    return constraint.evaluate(piecesCount);
}

bool Evaluator::ConstraintSatisfied() const
{
    return EvaluateConstrains() == 1;
}

double Evaluator::normalize(int value, int min, int max) const {
    return (value - min) / (max - min);
}

double Evaluator::predict() {
    // return random number from <0, 1>
    //  return ((double)rand() / (double)RAND_MAX);
    // ----------------- OLD EVALUATION -------------------------------------------------------------------------------

    // // WARNING tensor sharing memory with board array
    // torch::Tensor sharedBoardTensor = board.get2dTensor();
    // // because of batches making i need to copy the board tensor - board array (so board tensor) is updating
    // torch::Tensor boardTensor = sharedBoardTensor.clone();

    torch::Device device("cuda:0");


    uint64_t bitboard_array[12];
    board.toBitboard(bitboard_array);
    Bitboard bitboard;
    for ( int i = 0; i < 12; i++ )
        bitboard.binPlanes[i] = bitboard_array[i];

    torch::Tensor bitboardTensor = torch::zeros({6*2, 8, 8}, torch::kUInt8);
    bitboard.toTensor(bitboardTensor);
    bitboardTensor = bitboardTensor.to(device);

    torch::Tensor constraintAlgebraic = torch::zeros({5, 8, 8}, torch::kUInt8);
    constraint.toAlgebraicTensor(constraintAlgebraic);
    constraintAlgebraic = constraintAlgebraic.to(device);

    auto input = torch::cat({bitboardTensor, constraintAlgebraic}, 0).unsqueeze(0);
    // cout << input.sizes() << endl;

    // vector<torch::jit::IValue> inputs;
    // inputs.emplace_back(boardTensor);
    // inputs.emplace_back(constraintTensor);
    torch::Tensor output = net->forward(input);
    // cout << output.sizes() << endl;
    //cout << output.item<double>() << endl;
    return output.item<double>();
}
