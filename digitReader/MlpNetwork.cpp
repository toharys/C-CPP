#include "MlpNetwork.h"
#define BUILD(w,b,i) Dense(w[i],b[i],activation::relu)
#define BUILD_LAST(w,b,i) Dense(w[i],b[i],activation::softmax)
#define ZERO 0
#define ONE 1
#define TWO 2
#define THREE 3

/**
 * gets four dense objects and builds the network
 * @param weights
 * @param biases
 */
MlpNetwork::MlpNetwork(Matrix* weights, Matrix* biases):
        _layers{BUILD(weights,biases,ZERO),
                BUILD(weights,biases,ONE),
                BUILD(weights,biases,TWO),
                BUILD_LAST(weights,biases,THREE)}
{}
/**
 * by the layers dense's calculates the final network output
 * @param img
 * @return the digit with the greatest probability in the output vector
 */
digit MlpNetwork::operator() (Matrix& img) const
{   Matrix output_matrix = _layers[ZERO](img);
    for (int i = 1; i < MLP_SIZE; ++i)
    {   // in each layer insert as input the last output's layer and save
        // the new one as the new output
        output_matrix = _layers[i](output_matrix);
    }
    digit res = {0,0};
    for (int i = 0; i < output_matrix.get_rows(); ++i)
    {   // finds the digit with the max probability
        if (output_matrix[i]>res.probability)
        {
            res.value = i;
            res.probability = output_matrix[i];
        }
    }
    return res;
}

