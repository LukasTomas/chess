#pragma once

#include "ResidualBlock.h"

#include <torch/torch.h>
#include <vector>

class ResNetImpl : public torch::nn::Module {
public:
    ResNetImpl( int64_t inputPlanes, size_t residualBlocksN, bool useValueConvolution = false);

    torch::Tensor forward( torch::Tensor input );

private:

    bool _useValueConvolution;

    torch::nn::Sequential input_cv{nullptr};
    std::vector<ResidualBlock> residualBlocks;
    torch::nn::Sequential valueConv{nullptr};
    torch::nn::Linear lin{nullptr};
};

TORCH_MODULE(ResNet);
