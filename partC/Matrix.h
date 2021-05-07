#ifndef EX3_Matrix
#define EX3_Matrix
#include "Exceptions.h"
#include "Auxiliaries.h"
#include <string>
#define DIAGONAL_FIX 1
#define VALID_COMPARE_VALUE true

namespace mtm
{
    template <class T>
    class Matrix
    {
        Dimensions dims;
        T *matrix_values;

        static bool IsIllegalInitialization(Dimensions dims);
        static bool ValidDimension(Dimensions dims1, Dimensions dims2);
        bool TryAccessIllegalElement(const int row, const int col) const;
        Matrix<bool> compareMatrix(bool compareFunc(const T, const T), const Matrix matrix, const T alpha) const;
        static bool compareBigAndEqual(const T matrix_value, const T alpha);
        static bool compareBig(const T matrix_value, const T alpha);
        static bool compareSmallAndEqual(const T matrix_value, const T alpha);
        static bool compareSmall(const T matrix_value, const T alpha);
        static bool compareEqual(const T matrix_value, const T alpha);
        static bool compareNotEqual(const T matrix_value, const T alpha);

    public:
        Matrix(Dimensions dims, T initial_value = T());
        Matrix(const Matrix &matrix);
        Matrix &operator=(const Matrix &matrix);
        ~Matrix();
        static Matrix Diagonal(int dim, T value);
        int height() const;
        int width() const;
        int size() const;
        Matrix transpose() const;
        Matrix operator-() const;
        Matrix &operator+=(const T alpha);
        friend std::ostream &operator<<(std::ostream &os, const Matrix<T> &matrix)
        {
            printMatrix(os, matrix.begin(), matrix.end(), matrix.width());
            return os;
        }
        /*
        difines a semetric sum of two matrices return a copy of the sum
        assumptions: def' ctor' of T, operator = of T, operator + of T
        */
        friend Matrix<T> operator+(const Matrix<T> matrix_a, const Matrix<T> matrix_b)
        {
            if (!ValidDimension(matrix_a.dims, matrix_b.dims))
            {
                throw Matrix<T>::DimensionMismatch(matrix_a.dims, matrix_b.dims);
            }
            Matrix<T> sum_matrix(Dimensions(matrix_a.height(), matrix_a.width()));
            for (int i = 0; i < sum_matrix.height() * sum_matrix.width(); i++)
            {
                sum_matrix.matrix_values[i] = matrix_a.matrix_values[i] + matrix_b.matrix_values[i];
            }
            return sum_matrix;
        }
        /*define subtraction between two matrices return a copy of the result
        assumptions: def' ctor' of T, operator = of T, operator + of T, operator (onury) - of T*/
        friend Matrix<T> operator-(const Matrix<T> matrix_a, const Matrix<T> matrix_b)
        {
            if (!ValidDimension(matrix_a.dims, matrix_b.dims))
            {
                throw Matrix<T>::DimensionMismatch(matrix_a.dims, matrix_b.dims);
            }
            return matrix_a + (-matrix_b);
        }
        T &operator()(const int i, const int j);
        const T operator()(const int i, const int j) const;
        Matrix<bool> operator<(const T alpha) const;
        Matrix<bool> operator<=(const T alpha) const;
        Matrix<bool> operator>(const T alpha) const;
        Matrix<bool> operator>=(const T alpha) const;
        Matrix<bool> operator==(const T alpha) const;
        Matrix<bool> operator!=(const T alpha) const;
        template <class Condition>
        Matrix<T> apply(Condition function) const;
        //makes iterator class a member of the Matrix class
        class iterator;
        //return an iterator object that points to the first cell of the matrix
        Matrix::iterator begin()
        {
            return iterator(this, 0);
        }
        //return an itetaror object that points to the last cell of the matrix
        Matrix::iterator end()
        {
            return iterator(this, (*this).size());
        }
        //make const_iterator class a member of the Matrix class
        class const_iterator;

        Matrix::const_iterator begin() const
        {
            return const_iterator(this, 0);
        }
        Matrix::const_iterator end() const
        {
            return const_iterator(this, (*this).size());
        }

        class AccessIllegalElement : public Exception
        {
        public:
                AccessIllegalElement(): Exception("Mtm matrix error: An attempt to access an illegal element")
                {}
        };
        class IllegalInitialization : public Exception
        {
        public:
            IllegalInitialization(): Exception("Mtm matrix error: Illegal initialization values")
                {}
        };
        class DimensionMismatch : public Exception
        {
        private:
            Dimensions dim1;
            Dimensions dim2;

        public:
            DimensionMismatch(Dimensions dims1, Dimensions dims2) : 
            Exception("Mtm matrix error: Dimension mismatch: (" + std::to_string(dims1.getRow()) + "," + 
                std::to_string(dims1.getCol()) + ") (" + std::to_string(dims2.getRow()) 
                + "," + std::to_string(dims2.getCol()) + ")"),dim1(dims1), dim2(dims2)
            {}
        };
    };
    /*
    constructor, return a matrix initilized with the given initial object from T type,
    if the object was not supported use the defualt con' of the object T to ititliaze the matrix
    assumptions: con' , operator =
    */
    template <class T>
    Matrix<T>::Matrix(Dimensions dims, T initial_value) : dims(dims)
    {
        if (IsIllegalInitialization(dims))
        {
            throw IllegalInitialization();
        }
        matrix_values = new T[dims.getRow() * dims.getCol()];
        for (int i = 0; i < dims.getRow() * dims.getCol(); i++)
        {
            matrix_values[i] = initial_value;
        }
    }
    /*
    return a copy of the given matrix, copy each cell object
    assumptions: con' of T , operator = of T
    */
    template <class T>
    Matrix<T>::Matrix(const Matrix &matrix) : dims(matrix.dims), matrix_values(new T[matrix.dims.getRow() * matrix.dims.getCol()])
    {
        for (int i = 0; i < matrix.dims.getRow() * matrix.dims.getCol(); i++)
        {
            matrix_values[i] = matrix.matrix_values[i];
        }
    }

    /*
    assumptions: con' of T, operator = of T,
    */
    template <class T>
    Matrix<T> &Matrix<T>::operator=(const Matrix &matrix)
    {
        T *new_matrix_values = new T[matrix.dims.getRow() * matrix.dims.getCol()];
        delete[] matrix_values;
        matrix_values = new_matrix_values;
        for (int i = 0; i < matrix.dims.getRow() * matrix.dims.getCol(); i++)
        {
            matrix_values[i] = matrix.matrix_values[i];
        }
        dims = matrix.dims;
        return *this;
    }
    /*
     assumptions:  def' dtor' of T
    */
    template <class T>
    Matrix<T>::~Matrix<T>()
    {
        delete[] matrix_values;
    }
    /*
    return a diagonal marix in size of (dim,dim) with the def' object of T in the cells not on the diagonal
    assumptions: def' ctor' of T, operator = of T
    */
    template <class T>
    Matrix<T> Matrix<T>::Diagonal(int dim, T value)
    {
        if (IsIllegalInitialization(Dimensions(dim, dim)))
        {
            throw IllegalInitialization();
        }
        Matrix diagonal = Matrix(Dimensions(dim, dim)); //ititlize matrix with def' value of T
        for (int i = 0; i < dim * dim; i += dim + DIAGONAL_FIX)
        { //the place of the 1 on the diagonal grows by one at each iteration
            diagonal.matrix_values[i] = value;
        }
        return diagonal;
    }

    //return false if the dimensions are negative or zeros
    template <class T>
    bool Matrix<T>::IsIllegalInitialization(Dimensions dims)
    {
        return (dims.getRow() <= 0 || dims.getCol() <= 0);
    }
    //return false if the the dimensions are incompitable
    template <class T>
    bool Matrix<T>::ValidDimension(Dimensions dims1, Dimensions dims2)
    {
        return dims1.getRow() == dims2.getRow() && dims1.getCol() == dims2.getCol();
    }

    template <class T>
    bool Matrix<T>::TryAccessIllegalElement(const int row, const int col) const
    {
        return (row < 0 || col < 0 || row >= (*this).height() || col >= (*this).width());
    }
    //return the number of lines in the matrix
    template <class T>
    int Matrix<T>::height() const
    {
        return dims.getRow();
    }
    //return the number of cloums in the matrix
    template <class T>
    int Matrix<T>::width() const
    {
        return dims.getCol();
    }
    //return the number of cells in the matrix
    template <class T>
    int Matrix<T>::size() const
    {
        return dims.getRow() * dims.getCol();
    }
    /*return a transposed copy of the matrix
    assumptions: def' ctor' of T, operator = of T
    */
    template <class T>
    Matrix<T> Matrix<T>::transpose() const
    {
        Matrix transposed_matrix(Dimensions((*this).width(), (*this).height()));
        T *transposed_matrix_values = transposed_matrix.matrix_values;
        int row_length = (*this).width();
        int col_length = (*this).height();
        for (int i = 0; i < row_length; i++) //copy the values from the original to the transposed matrix
        {
            for (int j = 0; j < col_length; j++)
            {
                *(transposed_matrix_values + (i)*col_length + (j)) = *(matrix_values + (j)*row_length + (i));
            }
        }
        return transposed_matrix;
    }

    /*
    return a negative copy of the matrix
    assumptions: def' ctor' of T, operator - (onury) of T, operator = of T
    */
    template <class T>
    Matrix<T> Matrix<T>::operator-() const
    {
        Matrix<T> negative_matrix(Dimensions((*this).height(), (*this).width()));
        for (int i = 0; i < negative_matrix.height() * negative_matrix.width(); i++)
        {
            negative_matrix.matrix_values[i] = (-matrix_values[i]);
        }
        return negative_matrix;
    }

    /*define a sum of scalar + matrix:by adding and the scalar to to each cell of the matrix
    assumptions: def' ctor' of T, operator = of T, operator + of T
    */
    template <class T>
    Matrix<T> operator+(const T alpha, const Matrix<T> matrix)
    {
        Matrix<T> alpha_matrix(Dimensions(matrix.height(), matrix.width()), alpha);
        return alpha_matrix + matrix; //return a copy of the sum of the two
    }
    /*define a sum of matrix + scalar:by adding and the scalar to to each cell of the matrix
    assumptions: def' ctor' of T, operator = of T, operator + of T
    */
    template <class T>
    Matrix<T> operator+(const Matrix<T> matrix, const T alpha)
    {
        Matrix<T> alpha_matrix(Dimensions(matrix.height(), matrix.width()), alpha);
        return matrix + alpha_matrix;
    }

    /*gets a scalar and add it to every argument of the matrix return a refernce to the matrix
    assumptions: def' ctor' of T, operator = of T, operator + of T*/
    template <class T>
    Matrix<T> &Matrix<T>::operator+=(const T alpha)
    {
        (*this) =  *(this)+alpha; //update the matrix to be the sum
        return *this;
    }

    //get a row and a colum and return a pointer to the specified cell
    template <class T>
    T &Matrix<T>::operator()(const int i, const int j)
    {
        if ((*this).TryAccessIllegalElement(i, j))
        {
            throw(AccessIllegalElement());
        }
        int col = (*this).dims.getCol();
        return (*this).matrix_values[col * i + j];
    }
    //get a row and a coulom and return the value of the specified cell
    template <class T>
    const T Matrix<T>::operator()(const int i, const int j) const
    {
        if ((*this).TryAccessIllegalElement(i, j))
        {
            throw(AccessIllegalElement());
        }
        int col = (*this).dims.getCol();
        const T returned_value = (*this).matrix_values[col * i + j]; //return the value in the 2 dimensional array
        return returned_value;
    }
    //return a Matrix<bool> with 0 where the compareFunction return false and 1 for true
    //compareFunction is a pointer to a compare function
    template <class T>
    Matrix<bool> Matrix<T>::compareMatrix(bool compareFunc(const T, const T),
     const Matrix<T> matrix, const T alpha) const
    {
        Matrix<bool> result(Dimensions(matrix.height(), matrix.width()), false); //matix is initilized with zeros
        for (int i = 0; i < matrix.height(); i++)
        {
            for (int j = 0; j < matrix.width(); j++)
            {
                if (compareFunc(matrix(i, j), alpha))
                {
                    result(i, j) = VALID_COMPARE_VALUE; //set the value to one where the condition is true
                }
            }
        }
        return result;
    }
    //return true if the given T object is bigger or equal to the matrix value in a specific cell
    //assumptions: operator < of T
    template <class T>
    bool Matrix<T>::compareBigAndEqual(const T matrix_value, const T alpha)
    {
        return !(matrix_value < alpha);
    }

    //return true if the given T object is bigger of the matrix value in a specific cell
    //assumptions: operator > of T
    template <class T>
    bool Matrix<T>::compareBig(const T matrix_value, const T alpha)
    {
        return (matrix_value > alpha);
    }
    //return true if the given T object is smaller or equal to the matrix value in a specific cell
    //assumptions: operator > of T
    template <class T>
    bool Matrix<T>::compareSmallAndEqual(const T matrix_value, const T alpha)
    {
        return !(matrix_value > alpha);
    }
    //return true if the given T object is smaller of the matrix value in a specific cell
    //assumptions: operator < of T
    template <class T>
    bool Matrix<T>::compareSmall(const T matrix_value, const T alpha)
    {
        return (matrix_value < alpha);
    }
    //return true if the given T object is equal of the the matrix value in a specific cell
    //assumptions: operator == of
    template <class T>
    bool Matrix<T>::compareEqual(const T matrix_value, const T alpha)
    {
        return (matrix_value == alpha);
    }
    //return true if the given T object is not equal of the matrix value in a specific cell
    //assumptions: operator == of T
    template <class T>
    bool Matrix<T>::compareNotEqual(const T matrix_value, const T alpha)
    {
        return !(matrix_value == alpha);
    }

    //return true if all the matrix args are different that 0 otherwise flase
    template <class T>
    bool all(const Matrix<T> matrix)
    {
        for (int i = 0; i < matrix.height(); i++)
        {
            for (int j = 0; j < matrix.width(); j++)
            {
                if ((bool)matrix(i, j) == false)
                { //found a vaule equals to false
                    return false;
                }
            }
        }
        return true; //found no vaules equals to zero
    }
    //return true if one of the matrix args are different then 0 otherwise flase
    template <class T>
    bool any(const Matrix<T> matrix)
    {
        for (int i = 0; i < matrix.height(); i++)
        {
            for (int j = 0; j < matrix.width(); j++)
            {
                if ((bool)matrix(i, j) != false) //found a vaule different from zero
                    return true;
            }
        }
        return false; //found no values different from false
    }
    //get a T object and retrun a boolean matrix with false where the value were bigger and true where smaller
    //assumptions: operator < of T
    template <class T>
    Matrix<bool> Matrix<T>::operator<(const T alpha) const
    {
        return Matrix<T>::compareMatrix(Matrix<T>::compareSmall, (*this), alpha); //passing a compare function
    }
    //get a T object and retrun a boolean matrix with false where the value were bigger and true where smaller or equal
    //assumptions: operator > of T
    template <class T>
    Matrix<bool> Matrix<T>::operator<=(const T alpha) const
    {
        return Matrix<T>::compareMatrix(Matrix<T>::compareSmallAndEqual, (*this), alpha); //passing a compare function
    }
    //get a T object and retrun a boolean matrix with false where the value were smaller and true where bigger
    //assumptions: operator > of T
    template <class T>
    Matrix<bool> Matrix<T>::operator>(const T alpha) const
    {
        return Matrix<T>::compareMatrix(Matrix<T>::compareBig, (*this), alpha); //passing a compare function
    }
    //get a T object and retrun a boolean matrix with false where the value were smaller and true where bigger or equal
    //assumptions: operator < of T
    template <class T>
    Matrix<bool> Matrix<T>::operator>=(const T alpha) const
    {
        return Matrix<T>::compareMatrix(Matrix<T>::compareBigAndEqual, (*this), alpha); //passing a compare function
    }
    //get a T object and retrun a boolean matrix with false where the value were different and true where equal
    //assumptions: operator == of
    template <class T>
    Matrix<bool> Matrix<T>::operator==(const T alpha) const
    {
        return Matrix<T>::compareMatrix(Matrix<T>::compareEqual, (*this), alpha); //passing a compare function
    }
    //get a T object and retrun a boolean matrix with false where the value were equal and true where differeny
    //assumptions: operator == of T
    template <class T>
    Matrix<bool> Matrix<T>::operator!=(const T alpha) const
    {
        return Matrix<T>::compareMatrix(Matrix<T>::compareNotEqual, (*this), alpha); //passing a compare function
    }

    //implimention of iterator class
    template <class T>
    class Matrix<T>::iterator
    {
        Matrix<T> *matrix; //the object the itartor points on
        int index;         //the current index that the iterator points to
        //constrator
        iterator(Matrix<T> *matrix, int index) : matrix(matrix), index(index) {}
        //in order to accsess Matrix members
        friend class Matrix<T>;

    public:
        //return the value of the current place the iterator points to
        T &operator*() const
        {
            if (index < 0 || index >= matrix->size())
            {
                throw(AccessIllegalElement());
            }
            return matrix->matrix_values[index];
        }
        //progress the iterator to point on the next object and retrun the iterator
        iterator &operator++()
        {
            ++index;
            return *this;
        }
        //progress the iterator to point on the next object but return the same iterator before progression
        iterator operator++(int)
        {
            iterator result = *this;
            ++*this;
            return result;
        }
        //return true if the iterators points to the same place otherwise return flase
        bool operator==(const iterator &it) const
        {
            // assert(matrix == it.matrix); //iterators need to point to the same object
            return index == it.index;
        }
        //return false if the iterators points to the same place otherwise return true
        bool operator!=(const iterator &it) const
        {
            return !(*this == it);
        }
        //using default copy constractor
        iterator(const iterator &) = default;
        //usinf default operator of =
        iterator &operator=(const iterator &) = default;
    };

    // //implimention of const iterator class
    template <class T>
    class Matrix<T>::const_iterator
    {
        const Matrix *matrix;                                                //points on a read only matrix
        int index;                                                           //the current place the iterator points to
        const_iterator(const Matrix<T> *matrix, int index) : matrix(matrix), //constractor
                                                             index(index)
        {
        }
        friend class Matrix<T>; //gain access to Matrix members

    public:
        //return the value the iterator points to
        const T &operator*() const
        {
            if (index < 0 || index >= matrix->size())
            {
                throw(AccessIllegalElement());
            }
            return matrix->matrix_values[index];
        }
        //return an iterator pointing to the next value
        const_iterator &operator++()
        {
            ++index;
            return *this;
        }
        //return an iterator pointing to the same value as before but progress it at the end of the line
        const_iterator operator++(int)
        {
            const_iterator result = *this;
            ++*this;
            return result;
        }
        //return true if the iterators points to the same places in the matrices otherwise false
        bool operator==(const const_iterator &it) const
        {
            //   assert(matrix == it.matrix); //iterators need to point to the same object
            return index == it.index;
        }
        //return false if the iterators points to the same places in the matrices otherwise true
        bool operator!=(const const_iterator &it) const
        {
            return !(*this == it);
        }
        //usinf the difault copy constractor
        const_iterator(const const_iterator &) = default;
        //using the difualt = operator
        const_iterator &operator=(const const_iterator &) = default;
    };
    template <class T>
    template <class Condition>
    Matrix<T> Matrix<T>::apply(Condition function) const
    {
        Matrix<T> result((*this).dims);
        //     const_iterator it1 = (*this).begin();
        //     iterator it2 = result.begin();
        //     for (; it1 != (*this).end(), it2 != result.end(); it1++, it2++)
        //     {
        //         *it2 = function(*it1);
        //     }

        for (int i = 0; i < (*this).size(); i++)
        {
            result.matrix_values[i] = function((*this).matrix_values[i]);
        }
        return result;
    }

} // namespace mtm
#endif //EX3_Matrix