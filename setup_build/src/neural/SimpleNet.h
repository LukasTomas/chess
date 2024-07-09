#ifndef __ONELAYERNET_H__
#define __ONELAYERNET_H__

#include <torch/torch.h>

class SimpleNetImpl : public torch::nn::Module {
public:

    SimpleNetImpl (int64_t boardSize=64, int64_t constraintsSize=5);


    torch::Tensor forward(torch::Tensor & input);

private:
    torch::nn::Linear fc1{nullptr}; // Adjust input size for the chessboard
    torch::nn::Linear fc2{nullptr}; // Adjust input size for the combined features
    torch::nn::Linear fc3{nullptr};
    torch::nn::Linear fc4{nullptr};
    torch::nn::Linear fc5{nullptr};

    torch::nn::Dropout dropout{nullptr};

    torch::nn::Linear fc_board{nullptr}, fc_constraints{nullptr}, fc_combined{nullptr}, fc_combined_2{nullptr};

};

TORCH_MODULE(SimpleNet);

#endif
