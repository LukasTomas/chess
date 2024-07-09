#ifndef __CONVNET_H__
#define __CONVNET_H__

#include <torch/torch.h>

class ConvNetImpl : public torch::nn::Module {
public:

    ConvNetImpl (int64_t boardSize=64, int64_t constraintsSize=5);

    torch::Tensor forward(torch::Tensor & input);

private:
    torch::nn::Conv2d conv1{nullptr}, conv1_5{nullptr}, conv2{nullptr};
    torch::nn::Linear fc1{nullptr}, fc2{nullptr}, fc3{nullptr};
    torch::nn::Dropout dropout{nullptr};

};

TORCH_MODULE(ConvNet);

#endif
