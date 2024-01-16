#include "Matrix.h"
#ifndef ACTIVATION_H
#define ACTIVATION_H

namespace activation
{
    Matrix relu(const Matrix& m);
    Matrix softmax(const Matrix& m);
}

typedef Matrix (*ActivationFunction)(const Matrix&);

#endif //ACTIVATION_H