#include "ResidualBlock.h"

#include <iostream>
using namespace std;

ResidualBlockImpl::ResidualBlockImpl( int64_t inputPlanes, int64_t outputPlanes, int64_t kernelSize, int64_t stride, int64_t padding ) {
    cv1 = register_module("conv1", torch::nn::Sequential(
        torch::nn::Conv2d(torch::nn::Conv2dOptions(inputPlanes, outputPlanes, kernelSize).stride(stride).padding(padding)),
        torch::nn::BatchNorm2d(outputPlanes),
        torch::nn::ReLU()
    ));

    cv2 = register_module("conv2", torch::nn::Sequential(
        torch::nn::Conv2d(torch::nn::Conv2dOptions(outputPlanes, outputPlanes, kernelSize).stride(stride).padding(padding)),
        torch::nn::BatchNorm2d(outputPlanes)
    ));
}



torch::Tensor ResidualBlockImpl::forward( torch::Tensor input ) {
    torch::Tensor residual = input;

    input = cv1->forward(input);
    input = cv2->forward(input);

    input += residual;
    input = torch::relu(input);

    // cout << "\tResidual Block in chain" << endl;

    return input;
}
