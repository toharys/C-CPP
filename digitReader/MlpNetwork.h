//MlpNetwork.h

#ifndef MLPNETWORK_H
#define MLPNETWORK_H

#include "Dense.h"


#define MLP_SIZE 4

/**
 * @struct digit
 * @brief Identified (by Mlp network) digit with
 *        the associated probability.
 * @var value - Identified digit value
 * @var probability - identification probability
 */
typedef struct digit {
	unsigned int value;
	float probability;
} digit;

const matrix_dims img_dims = {28, 28}; // image's matrix dims
const matrix_dims weights_dims[] = {{128, 784}, // each level
									{64,  128}, // and it's
									{20,  64}, // weight matrix
									{10,  20}}; // dims
const matrix_dims bias_dims[] = {{128, 1},  // the same just
								 {64,  1},  // with bias vectors
								 {20,  1},
								 {10,  1}};

class MlpNetwork
{
    Dense _layers[MLP_SIZE];

public:
    MlpNetwork(Matrix* weights, Matrix* biases);
    digit operator() (Matrix& img) const;
};

#endif // MLPNETWORK_H