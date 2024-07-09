#pragma once

#include <torch/torch.h>

class ResidualBlockImpl : public torch::nn::Module {
public:

    ResidualBlockImpl( int64_t inputPlanes, int64_t outputPlanes, int64_t kernelSize=3, int64_t stride=1, int64_t padding=1  );

    torch::Tensor forward( torch::Tensor input );

private:

    torch::nn::Sequential cv1{nullptr}, cv2{nullptr};

    // torch::nn::Conv2d conv1{nullptr}, conv2{nullptr};
    // torch::nn::BatchNorm2d batchNorm1{nullptr}, batchNorm2{nullptr};

};

TORCH_MODULE(ResidualBlock);
