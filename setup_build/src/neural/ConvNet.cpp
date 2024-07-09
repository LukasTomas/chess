#include "ConvNet.h"
#include <iostream>
using namespace std;

// Define a deep neural network architecture with multi-inputs
ConvNetImpl::ConvNetImpl(int64_t boardSize, int64_t constraintsSize) {
    conv1 = register_module("conv1", torch::nn::Conv2d(17, 64, 3));
    conv1_5 = register_module("conv1_5", torch::nn::Conv2d(64, 64, 3));
    conv2 = register_module("conv2", torch::nn::Conv2d(64, 32, 3));

    // fc1 = register_module("fc1", torch::nn::Linear(128, 128)); // Adjust input size for the chessboard
    fc2 = register_module("fc2", torch::nn::Linear(128, 64)); // Adjust input size for the combined features
    fc3 = register_module("fc3", torch::nn::Linear(64, 1));

    dropout = register_module("dropout", torch::nn::Dropout(0.5));
}

    // Implement the forward pass function
torch::Tensor ConvNetImpl::forward(torch::Tensor & input) {
    input = input.to(torch::kFloat32);

    // cout << input.sizes() << endl;
    input = torch::relu(conv1->forward(input));
    // cout << input.sizes() << endl;

    input = torch::relu(conv1_5->forward(input));
    // cout << input.sizes() << endl;


    input = torch::relu(conv2->forward(input));
    // cout << input.sizes() << endl;

    // input = torch::max_pool2d(input, 2);
    // cout << input.sizes() << endl;

    torch::nn::Flatten flat;
    input = flat(input);
    // cout << input.sizes() << endl;

    // input = input.view({-1, 16384});
    // input = torch::relu(fc1->forward(input));

    input = fc2->forward(input);
    input = fc3->forward(input);

    // y = dropout->forward(y);


    return torch::sigmoid(input);
}
