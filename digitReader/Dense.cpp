#include "Dense.h"

#define BIAS_MSG "invalid parameter: bias must be vector (one column)"
/**
 * gets as parameters the Dense members and build the curr dense
 * @param weights
 * @param bias
 * @param func
 */
Dense::Dense(Matrix& weights, Matrix& bias, ActivationFunction func):
    _weights(weights), _bias(bias), _func(func)
{
    if (bias.get_cols()!=1)
    {
        throw std::runtime_error(BIAS_MSG);
    }
}
/**
 *
 * @return the weight matrix
 */
Matrix Dense::get_weights() const
{
    return this->_weights;
}
/**
 *
 * @return the bias vector
 */
Matrix Dense::get_bias() const
{
    return this->_bias;
}
/**
 *
 * @return the activation function
 */
ActivationFunction Dense::get_activation() const
{
    return this->_func;
}
/**
 * calculates the dense output
 * @param input
 * @return calculated output
 */
Matrix Dense::operator() (const Matrix& input) const
{
    return _func((_weights*input)+_bias);
}