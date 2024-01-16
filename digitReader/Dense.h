#ifndef DENSE_H
#define DENSE_H

#include "Activation.h"

class Dense
{
    Matrix _weights;
    Matrix _bias;
    ActivationFunction _func;

public:
    Dense(Matrix& weights, Matrix& bias, ActivationFunction func);
    Matrix get_weights() const;
    Matrix get_bias() const;
    ActivationFunction get_activation() const;
    Matrix operator() (const Matrix& input) const;
};


#endif //DENSE_H
