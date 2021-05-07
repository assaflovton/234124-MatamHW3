#include "IntMatrix.h"
#include <assert.h>
using namespace mtm;
using std::cout;
#define IDENTITY_DIAGONAL_VALUE 1
#define VALID_COMPARE_VALUE 1
#define DIAGONAL_FIX 1


IntMatrix::IntMatrix(Dimensions dims, int initial_value) : 
dims(dims), matrix_values(new int[dims.getRow() * dims.getCol()])
{
    for (int i = 0; i < dims.getRow() * dims.getCol(); i++)
    {
        matrix_values[i] = initial_value;
    }
}

IntMatrix::IntMatrix(const IntMatrix &matrix) : 
dims(matrix.dims), matrix_values(new int[matrix.dims.getRow() * matrix.dims.getCol()])
{
    for (int i = 0; i < matrix.dims.getRow() * matrix.dims.getCol(); i++)
    {
        matrix_values[i] = matrix.matrix_values[i];
    }
}
IntMatrix &IntMatrix::operator=(const IntMatrix &matrix)
{
    if (this == &matrix) //check if it is the same object
    {
        return *this;
    }
    delete[] matrix_values;                                               //free the previous data
    dims = matrix.dims;                                                   // update the matrix dimesions
    matrix_values = new int[matrix.dims.getRow() * matrix.dims.getCol()]; //build new matrix in the new dimension
    for (int i = 0; i < matrix.dims.getRow() * matrix.dims.getCol(); i++) //copy the values
    {
        matrix_values[i] = matrix.matrix_values[i];
    }
    return *this;
}
IntMatrix IntMatrix::Identity(int dim)
{
    IntMatrix identity = IntMatrix(Dimensions(dim, dim));   //intilize matrix with zeros
    for (int i = 0; i < dim * dim; i += dim + DIAGONAL_FIX) 
    {//the place of the 1 on the diagonal grows by one at each iteration
        identity.matrix_values[i] = IDENTITY_DIAGONAL_VALUE;
    }
    return identity;
}
IntMatrix::~IntMatrix()
{
    delete[] matrix_values;
}
int IntMatrix::height() const
{
    return dims.getRow();
}
int IntMatrix::width() const
{
    return dims.getCol();
}
int IntMatrix::size() const
{
    return dims.getCol() * dims.getRow();
}
IntMatrix IntMatrix::transpose() const
{
    IntMatrix transposed_matrix(Dimensions((*this).width(), (*this).height()));
    int *transposed_matrix_values = transposed_matrix.matrix_values;
    int row_length = (*this).width();
    int col_length = (*this).height();
    for (int i = 0; i < row_length; i++) //copy the values from the original to the transposed matrix
    {
        for (int j = 0; j < col_length; j++)
        {
            *(transposed_matrix_values + i * col_length + j) = *(matrix_values + j * row_length + i);
        }
    }
    return transposed_matrix;
}

IntMatrix mtm::operator+(const IntMatrix matrix_a, const IntMatrix matrix_b)
{
    IntMatrix sum_matrix(Dimensions(matrix_a.height(), matrix_a.width()));
    for (int i = 0; i < sum_matrix.height() * sum_matrix.width(); i++)
    {
        sum_matrix.matrix_values[i] = matrix_a.matrix_values[i] + matrix_b.matrix_values[i];
    }
    return sum_matrix;
}

IntMatrix IntMatrix::operator-() const
{
    IntMatrix negative_matrix(Dimensions((*this).height(), (*this).width()));
    for (int i = 0; i < negative_matrix.height() * negative_matrix.width(); i++)
    {
        negative_matrix.matrix_values[i] -= matrix_values[i]; //matrix initilized with zeros
    }
    return negative_matrix;
}
IntMatrix mtm::operator-(const IntMatrix matrix_a, const IntMatrix matrix_b)
{
    return (matrix_a) + (-matrix_b); //using the overloaded operators + (binary) and -(unari)
}

IntMatrix mtm::operator+(const int alpha, const IntMatrix matrix)
{//create a matrix initilized with the scaler
    IntMatrix alpha_matrix(Dimensions(matrix.height(), matrix.width()), alpha); 
    return alpha_matrix + matrix;     //return a copy of the sum of the two
}
IntMatrix mtm::operator+(const IntMatrix matrix, const int alpha) //makes the operator + semtric
{
    return alpha + matrix;
}

IntMatrix &IntMatrix::operator+=(const int alpha)
{
    (*this) = alpha + *(this); //update the matrix to be the sum
    return *this;
}

std::ostream &mtm::operator<<(std::ostream &os, const IntMatrix &matrix)
{

    os << mtm::printMatrix(matrix.matrix_values, matrix.dims);
    return os;
}

int &IntMatrix::operator()(const int i, const int j)
{
    int col = (*this).dims.getCol();
     return (*this).matrix_values[col * (i) + (j)]; 
}//return the pointer to the value that represnt the argument in a two dimensional array

const int IntMatrix::operator()(const int i, const int j) const
{
    int col = (*this).dims.getCol();
    const int returned_int = (*this).matrix_values[col * (i) + (j)]; //return the value in the 2 dimensional array
    return returned_int;
}
//return an IntMatrix with 0 where the compareFunction return false and 1 for true
//compareFunction is a pointer to a compare function 
IntMatrix IntMatrix::compareMatrix(bool compareFunction(const int,const int)
,const IntMatrix matrix,const int alpha) const
{
    IntMatrix result(Dimensions(matrix.height(), matrix.width())); //matix is initilized with zeros
    for (int i = 0; i < matrix.height(); i++)
    {
        for (int j = 0; j < matrix.width(); j++)
        {
            if (compareFunction(matrix(i, j), alpha))
            {
                result(i, j) = VALID_COMPARE_VALUE; //set the value to one where the condition is true
            }
        }
    }
    return result;
}
//return true if the given int is bigger or equal to the scalar
bool IntMatrix::compareBigAndEqual(const int matrix_value, const int alpha)
{
    return !(matrix_value < alpha);
}
//return true if the given int is bigger of the scalar
bool IntMatrix::compareBig(const int matrix_value, const int alpha)
{
    return (matrix_value > alpha);
}
//return true if the given int is smaller or equal to the scalar
bool IntMatrix::compareSmallAndEqual(const int matrix_value, const int alpha)
{
    return !(matrix_value > alpha);
}
//return true if the given int is smaller of the scalar
bool IntMatrix::compareSmall(const int matrix_value, const int alpha)
{
    return (matrix_value < alpha);
}
//return true if the given int is equal of the scalar
bool IntMatrix::compareEqual(const int matrix_value, const int alpha)
{
    return (matrix_value == alpha);
}
//return true if the given int not equal of the scalar
bool IntMatrix::compareNotEqual(const int matrix_value, const int alpha)
{
    return !(matrix_value == alpha);
}
IntMatrix IntMatrix::operator<(const int alpha) const
{
    return compareMatrix(compareSmall, (*this), alpha); //passing a compare function
}
IntMatrix IntMatrix::operator<=(const int alpha) const
{
    return compareMatrix(compareSmallAndEqual, (*this), alpha); //passing a compare function
}
IntMatrix IntMatrix::operator>(const int alpha) const
{
    return compareMatrix(compareBig, (*this), alpha); //passing a compare function
}
IntMatrix IntMatrix::operator>=(const int alpha) const
{
    return compareMatrix(compareBigAndEqual, (*this), alpha); //passing a compare function
}
IntMatrix IntMatrix::operator==(const int alpha) const
{
    return compareMatrix(compareEqual, (*this), alpha); //passing a compare function
}
IntMatrix IntMatrix::operator!=(const int alpha) const
{
    return compareMatrix(compareNotEqual, (*this), alpha); //passing a compare function
}

bool mtm::all(const IntMatrix matrix)
{
    for (int i = 0; i < matrix.height(); i++)
    {
        for (int j = 0; j < matrix.width(); j++)
        {
            if (matrix(i, j) == 0) //found a vaule equals to zero
                return false;
        }
    }
    return true; //found no vaules equals to zero
}
bool mtm::any(const IntMatrix matrix)
{
    for (int i = 0; i < matrix.height(); i++)
    {
        for (int j = 0; j < matrix.width(); j++)
        {
            if (matrix(i, j) != 0) //found a vaule different from zero
                return true;
        }
    }
    return false; //found no values different from zero
}

IntMatrix::iterator::iterator(IntMatrix *matrix, int index) : matrix(matrix),
                                                              index(index)//initiliztion list of iterator
{
}

int &IntMatrix::iterator::operator*() const
{
    assert(index >= 0 && index < matrix->size());
    return matrix->matrix_values[index];
}

IntMatrix::iterator &IntMatrix::iterator::operator++()
{
    ++index;
    return *this;
}

IntMatrix::iterator IntMatrix::iterator::operator++(int)
{
    iterator result = *this;
    ++*this;
    return result;
}

bool IntMatrix::iterator::operator==(const iterator &i) const
{
    assert(matrix == i.matrix);//iterators need to point to the same object
    return index == i.index;
}

bool IntMatrix::iterator::operator!=(const iterator &i) const
{
    return !(*this == i);
}

IntMatrix::iterator IntMatrix::begin()
{
    return iterator(this, 0);
}

IntMatrix::iterator IntMatrix::end()
{
    return iterator(this, (*this).size());
}

IntMatrix::const_iterator::const_iterator(const IntMatrix *matrix, int index) : matrix(matrix),
                                                                                index(index)
{
}

const int &IntMatrix::const_iterator::operator*() const
{
    assert(index >= 0 && index < matrix->size());
    return matrix->matrix_values[index];
}

IntMatrix::const_iterator &IntMatrix::const_iterator::operator++()
{
    ++index;
    return *this;
}

IntMatrix::const_iterator IntMatrix::const_iterator::operator++(int)
{
    const_iterator result = *this;
    ++*this;
    return result;
}

bool IntMatrix::const_iterator::operator==(const const_iterator &i) const
{
    assert(matrix == i.matrix);//iterators need to point to the same object
    return index == i.index;
}

bool IntMatrix::const_iterator::operator!=(const const_iterator &i) const
{
    return !(*this == i);
}

IntMatrix::const_iterator IntMatrix::begin() const
{
    return const_iterator(this, 0);
}

IntMatrix::const_iterator IntMatrix::end() const
{
    return const_iterator(this, (*this).size());
}
