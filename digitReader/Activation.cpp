#include "Activation.h"
#include <cmath>

#define ZERO_DIVISION "math error: division by 0"
/**
 * a method that calculates the relu func on given matrix
 * @param m
 * @return return the relu matrix output
 */
Matrix activation::relu(const Matrix& m)
{
    Matrix res = Matrix(m);
    for (int i = 0; i < m.get_rows()*m.get_cols(); ++i)
    {
     if(m[i]<0)
     {
         res[i]=0;
     }
    }
    return res;
}
/**
 * a method that calculates the softmax func on given matrix
 * @param m
 * @return return the relu matrix output
 */
Matrix activation::softmax(const Matrix& m)
{
    Matrix res = Matrix(m.get_rows(),m.get_cols());
    float divisor = 0;
    for (int i = 0; i < m.get_rows()*m.get_cols(); ++i)
    {
        res[i] = std::exp(m[i]); // calculate the component exponent
        divisor += std::exp(m[i]); // summ's the all component exponent
    }
    if (divisor==0)
    {
        throw std::runtime_error(ZERO_DIVISION);
    }
    return (1/divisor)*res;
}


