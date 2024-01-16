// Matrix.h
#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>

/**
 * @struct matrix_dims
 * @brief Matrix dimensions container. Used in MlpNetwork.h and main.cpp
 */
typedef struct matrix_dims
{
	int rows, cols;
} matrix_dims;

// Insert Matrix class here...
class Matrix
{
    float* _matrix;
    int _rows;
    int _cols;
public:
    // constructors
    Matrix(int rows, int cols);
    Matrix():Matrix(1,1){};
    Matrix(const Matrix& m);
    // destructor
    ~Matrix();
    // getters
    int get_rows() const;
    int get_cols() const;
    // functions
    Matrix& transpose();
    Matrix& vectorize();
    void plain_print() const;
    Matrix dot (const Matrix& m) const;
    float norm() const;
    // operators
    Matrix operator+ (const Matrix& m) const;
    Matrix& operator= (const Matrix& m);
    Matrix operator* (const Matrix& m) const; // matrix * matrix
    Matrix operator* (float c) const; // matrix * scalar
    friend Matrix operator* (float c, const Matrix& m); // scalar * matrix
    Matrix operator+= (const Matrix& m);
    const float& operator() (int i, int j) const; // access to index (i,j) data
    float& operator() (int i,int j); // access to index (i,j) reference
    const float& operator[] (int k) const; // access to index [k] data
    float& operator[] (int k); // access to index [k] reference
    friend std::ostream& operator<< (std::ostream& s, const Matrix& m);
    friend std::istream& operator>> (std::istream& s, Matrix& m);
};

#endif //MATRIX_H