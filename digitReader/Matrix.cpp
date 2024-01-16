#include "Matrix.h"
#include <cmath>

#define INDEX(x,y,row_length) ((x*row_length)+y)

#define LENGTH_MSG "invalid row's or column's length"
#define MULTIPLICATION_MSG "math error: impossible multiply matrices"
#define RANGE_MSG "invalid index (out of matrix range)"
#define INPUT_MSG "invalid input"
#define MIN_RES 0.1
#define FILE_MSG "size of input inappropriate to size of matrix"

// constructors
/**
 * a constructor that gets number of rows and cols and build the matrix
 * @param rows
 * @param cols
 */
Matrix::Matrix(int rows, int cols): _matrix(new float[rows*cols]),
                                    _rows(rows),_cols(cols)
{
    if (rows<=0 || cols<=0)
    {
        delete[] this->_matrix;
        throw std::length_error(LENGTH_MSG);
    }
    for(int i=0;i<rows*cols;i++)
    {
        _matrix[i] = 0.0;
    }
}
/**
 * a copy constructor, receive an exist matrix and build another by deep copy
 * of the given one
 * @param m
 */
Matrix::Matrix(const Matrix &m): _matrix(new float[(m._rows)*(m._cols)]),
        _rows(m._rows), _cols(m._cols)
{
    for (int i = 0; i <( this->_rows)*(this->_cols); ++i)
    {
        this->_matrix[i] = m._matrix[i];
    }
}

// destructor
/**
 * deletes the allocated data
 */
Matrix::~Matrix()
{
    delete[] this->_matrix;
}

// getters
/**
 *
 * @return the number of rows
 */
int Matrix::get_rows() const
{
    return this->_rows;
}
/**
 *
 * @return the number of columns
 */
int Matrix::get_cols() const
{
    return this->_cols;
}

//functions
/**
 * transpose the matrix
 * @return the transposed matrix
 */
Matrix& Matrix::transpose()
{
    // builds and inserts the transposed matrix
    auto* temp = new float [(this->_rows)*(this->_cols)];
    for (int i = 0; i < this->_rows; ++i)
    {
        for (int j = 0; j < this->_cols; ++j)
        {
         temp[INDEX(j,i,this->_rows)] = this->_matrix[INDEX(i,j,this->_cols)];
        }
    }
    delete[] this->_matrix;
    this->_matrix = temp;
    // swaps the new and old matrices
    int temp_cols = this->_cols;
    this->_cols = this->_rows;
    this->_rows = temp_cols;
    return *this;
}
/**
 * change the matrix to vector, by chaining it's rows
 * @return the vector
 */
Matrix& Matrix::vectorize()
{   // updates the matrix size to vector size
    this->_rows = (this->_rows)*(this->_cols);
    this->_cols = 1;
    return *this;
}
/**
 * prints the matrix data
 */
void Matrix::plain_print() const
{
    for (int i = 0; i < this->_rows; ++i)
    {
        for (int j = 0; j < this->_cols; ++j)
        {
            std::cout<<this->_matrix[INDEX(i,j,this->_cols)]<<" ";
        }
        std::cout<<std::endl;
    }

}
/**
 * receives other matrix and return the matrices dot scalar (multiplication
 * component component)
 * @param m
 * @return
 */
Matrix Matrix::dot(const Matrix& m) const
{   // validation check (matrix size)
    if (this->_rows!=m._rows || this->_cols!=m._cols)
    {
        throw std::length_error(LENGTH_MSG);
    }
    // build the res matrix as matrix which its components are
    // the multiplication of the tow given's components
    Matrix res = Matrix(this->_rows, this->_cols);
    for (int i = 0; i < this->_rows; ++i)
    {
        for (int j = 0; j < this->_cols; ++j)
        {
            res._matrix[INDEX(i,j,this->_cols)] =
                    this->_matrix[INDEX(i,j,this->_cols)] *
                    m._matrix[INDEX(i,j,this->_cols)];
        }
    }
    return res;
}
/**
 * calculate the norm of matrix
 * @return calculated norm
 */
float Matrix::norm() const
{
    float res = 0;
    for (int i = 0; i < this->_rows; ++i)
    {
        for (int j = 0; j < this->_cols; ++j)
        {
            res += (float)pow(this->_matrix[INDEX(i,j,this->_cols)],2);
        }
    }
    return res;
}

//operators
/**
 * operator that calculate the add of two matrices
 * @param m
 * @return the calculated add matrix
 */
Matrix Matrix::operator+(const Matrix& m) const
{   // validation check (matrices size)
    if (this->_rows!=m._rows || this->_cols!=m._cols)
    {
        throw std::length_error(LENGTH_MSG);
    }
    Matrix res = Matrix(this->_rows, this->_cols);
    for (int i = 0; i < this->_rows; ++i)
    { // adds var var of the matrices
        for (int j = 0; j < this->_cols; ++j)
        {
            res._matrix[INDEX(i,j,this->_cols)] =
                    this->_matrix[INDEX(i,j,this->_cols)] +
                    m._matrix[INDEX(i,j,this->_cols)];
        }
    }
    return res;
}
/**
 * operator that executed assignment to the matrix
 * @param m
 * @return the new assigned matrix
 */
Matrix& Matrix::operator=(const Matrix& m)
{
    if ( this==&m || m._matrix==this->_matrix)
    {   // checks self assignment
        return *this;
    }
    // builds the matrix to swap with old one and swaps it
    auto* res_mat = new float[m._cols*m._rows];
    for (int i=0;i<m._cols*m._rows;i++)
    {
        res_mat[i] = m._matrix[i];
    }
    delete[] this->_matrix;
    this->_matrix = res_mat;
    this->_cols = m._cols;
    this->_rows = m._rows;
    return *this;
}
/**
 * gets as a parameter other matrix and calculates matrix * other
 * @param m
 * @return the calculated matrix
 */
Matrix Matrix::operator*(const Matrix &m) const
{
    if(this->_cols != m._rows)
    {
        throw std::runtime_error(MULTIPLICATION_MSG);
    }
    // initialize res matrix in the fit size
    Matrix res = Matrix(this->_rows, m._cols);
    for (int i = 0; i < this->_rows; i++)
    { // for each row in a matrix
        for (int j=0;j<m._cols;j++)
        { // for each col in b matrix
            float summer = 0.0;
            for (int k=0; k<m._rows;k++)
            { // sigma(k=0->n): a(i,k)+b(k,j)
                summer+=
                        this->_matrix[INDEX(i,k,this->_cols)]*
                        m._matrix[INDEX(k,j,m._cols)];
            }
            res._matrix[INDEX(i,j,res._cols)] = summer;
        }
    }
    return res;
}
/**
 * gets as a parameter scalar and calculates matrix * scalar
 * @param c
 * @return the calculated matrix
 */
Matrix Matrix::operator*(float c) const
{
    Matrix res = Matrix(*this);
    for (int i = 0; i < this->_cols*this->_rows; ++i)
    {
     res._matrix[i] = c*this->_matrix[i];
    }
    return res;
}
/**
 *  gets as a parameter scalar and matrix, and calculates scalar * matrix
 * @param c
 * @param m
 * @return the calculated matrix
 */
Matrix operator* (float c, const Matrix& m)
{
    return m*c;
}
/**
 * gets as a parameter other matrix and calculates this+=other
 * @param m
 * @return the calculated matrix
 */
Matrix Matrix::operator+=(const Matrix &m)
{   // validation check
    if (this->_cols!=m._cols || this->_rows!=m._rows)
    {
        throw std::length_error(LENGTH_MSG);
    }

    for (int i = 0; i < this->_rows; ++i)
    {   // makes += to each matrix's var
        for (int j = 0; j < this->_cols; ++j)
        {
            this->_matrix[INDEX(i,j,this->_cols)] +=
                    m._matrix[INDEX(i,j,m._cols)];
        }
    }
    return *this;
}
/**
 * an operator that enables to get the matrix component in the place (i,j)
 * without the ability to change it
 * @param i
 * @param j
 * @return reference to the (i,j) component
 */
const float& Matrix::operator() (int i, int j) const
{
    if (i>=this->_rows || j>= this->_cols || i<0 || j<0)
    {
        throw std::out_of_range(RANGE_MSG);
    }
    return this->_matrix[INDEX(i,j,this->_cols)];
}
/**
 * an operator that enables to get the matrix component in the place (i,j)
 * with the ability to change it
 * @param i
 * @param j
 * @return
 */
float& Matrix::operator() (int i,int j)
{
    if (i>=this->_rows || j>= this->_cols || i<0 || j<0)
    {
        throw std::out_of_range(RANGE_MSG);
    }
    return this->_matrix[INDEX(i,j,this->_cols)];
}
/**
 * an operator that enables to get the matrix component in the place [k]
 * without the ability to change it
 * @param k
 * @return reference to the [k] component
 */
 const float& Matrix::operator[](int k) const
{
    if (k>=(this->_cols*this->_rows) || k<0)
    {
        throw std::out_of_range(RANGE_MSG);
    }
    return this->_matrix[k];
}
/**
 * an operator that enables to get the matrix component in the place [k]
 * with the ability to change it
 * @param k
 * @return
 */
float& Matrix::operator[](int k)
{
    if (k>=(this->_cols*this->_rows) || k<0)
    {
        throw std::out_of_range(RANGE_MSG);
    }
    return this->_matrix[k];
}
/**
 * a operator return a formatted stream of the image that the matrix resemble
 * @param s
 * @param m
 * @return the formatted stream
 */
std::ostream& operator<< (std::ostream& s, const Matrix& m)
{
    if (!s)
    {
        throw std::runtime_error(INPUT_MSG);
    }
    for (int i = 0; i < m._rows; ++i)
    {
        for (int j = 0; j < m._cols; ++j)
        {
            if(m(i,j)>MIN_RES)
            {
                s<<"**";
            }
            else
            {
                s<<"  ";
            }
        }
        s<<std::endl;
    }
    return s;
}
/**
 * an operator that gets by input stream the matrix components
 * @param is
 * @param m
 * @return the stream
 */
std::istream& operator>> (std::istream& is, Matrix& m)
{
    if (!is)
    {
        throw std::runtime_error(INPUT_MSG);
    }

    long int array_size_bytes = m._cols*m._rows * sizeof (float);
    is.seekg (0, std::ios_base::end);
    long int file_size_bytes = is.tellg();
    if (file_size_bytes != array_size_bytes)
    {   // if the given file isnt appropriate to matrix size throws exception
        throw std::runtime_error(FILE_MSG);
    }
    is.seekg (0, std::ios_base::beg);
    if (!is.read ((char *)m._matrix, array_size_bytes))
    {   // reads the file in one command (because the memory is continious
        // if failed throws exception
        throw std::runtime_error(FILE_MSG);
    }
    return is;
}




