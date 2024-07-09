#include "ResNet.h"

#include <iostream>
using namespace std;

ResNetImpl::ResNetImpl( int64_t inputPlanes, size_t residualBlocksN, bool useValueConvolution ) : _useValueConvolution(useValueConvolution) {
    const int filters = 256;

    input_cv = register_module("input_cv", torch::nn::Sequential(
        torch::nn::Conv2d(inputPlanes, filters, 3),
        torch::nn::BatchNorm2d(filters),
        torch::nn::ReLU()
    ));

    for ( int i = 0; i < residualBlocksN; i++ )
        residualBlocks.emplace_back( register_module("residualBlock" + std::to_string(i), ResidualBlock(filters, filters)) );

    if ( _useValueConvolution )
        valueConv = register_module("valueConv", torch::nn::Sequential(
            torch::nn::Conv2d(filters, 1, 1),
            torch::nn::BatchNorm2d(1),
            torch::nn::ReLU()
        ));

    int64_t linearInputSize = 6*6;
    if ( !_useValueConvolution )
        linearInputSize *= filters;
    lin = register_module("lin", torch::nn::Linear(linearInputSize, 1));
};

torch::Tensor ResNetImpl::forward( torch::Tensor input ) {
    input = input.to(torch::kFloat32);

    auto output = input_cv->forward(input);

    for ( int i = 0; i < residualBlocks.size(); i++ )
        output = residualBlocks[i]->forward(output);
    // cout << "ResNet chain ----------" << endl;

    if ( _useValueConvolution )
        output = valueConv->forward(output);

    torch::nn::Flatten flat;
    output = flat(output);

    output = lin->forward(output);
    return torch::softmax(output, 1);
}
