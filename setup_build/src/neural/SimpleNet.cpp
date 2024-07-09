#include "SimpleNet.h"

#include <iostream>
using namespace std;

SimpleNetImpl::SimpleNetImpl(int64_t boardSize, int64_t constraintsSize) {
    fc1 = register_module("fc1", torch::nn::Linear(12*boardSize + constraintsSize, 1024));
    fc3 = register_module("fc3", torch::nn::Linear(1024, 128));
    fc4 = register_module("fc4", torch::nn::Linear(128, 64));
    fc5 = register_module("fc5", torch::nn::Linear(64, 1));

    // fc_board = register_module("fc_board", torch::nn::Linear(boardSize, 2048)); // Input size: 64 (chessboard)
    // fc_constraints = register_module("fc_constraints", torch::nn::Linear(constraintsSize, 15)); // Input size: 5 (constraints)

    // fc_combined = register_module("fc_combined", torch::nn::Linear(527, 32)); // Combined input size: 32 (board) + 3 (constraints)
    // fc_combined_2 = register_module("fd_combined_2", torch::nn::Linear(32, 1)); // Combined input size: 32 (board) + 3 (constraints)

    dropout = register_module("dropout", torch::nn::Dropout(0.5));
}


torch::Tensor SimpleNetImpl::forward(torch::Tensor & input) {
    input = input.to(torch::kFloat32);
    //torch::Tensor input = torch::cat({board.to(torch::kFloat32), constraints.to(torch::kFloat32)}, /*dim=*/1);

    auto x = torch::relu(fc1->forward(input));
    //x = torch::relu(fc2->forward(x));
    x = torch::relu(fc3->forward(x));
    x = torch::relu(fc4->forward(x));
    x = torch::sigmoid(fc5->forward(x));

    return x;



    // auto board_relu = torch::relu(fc_board->forward(board.to(torch::kFloat32)));
    // auto constraints_relu = torch::relu(fc_constraints->forward(constraints.to(torch::kFloat32)));

    // torch::Tensor combined = torch::cat({board_relu, constraints_relu}, /*dim=*/1);

    // // combined = dropout->forward(combined);
    // combined = torch::relu(fc_combined->forward(combined));
    // combined = fc_combined_2->forward(combined);

    // return torch::sigmoid(combined);
}
