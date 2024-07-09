#pragma once

#include <torch/torch.h>

// Define a custom data structure to hold the board, constraints, and target
struct CustomExample {
    torch::Tensor board;
    torch::Tensor constraints;
    torch::Tensor target;
};
