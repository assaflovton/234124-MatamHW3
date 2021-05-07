#ifndef EX3_INTMATRIX
#define EX3_INTMATRIX
#include "Auxiliaries.h"
namespace mtm
{
    class IntMatrix
    {
        Dimensions dims;
        int *matrix_values;
        IntMatrix compareMatrix(bool compareFunc(const int, const int),
        const IntMatrix matrix,const int alpha) const;
        static bool compareBigAndEqual(const int matrix_value, const int alpha);
        static bool compareBig(const int matrix_value, const int alpha);
        static bool compareSmallAndEqual(const int matrix_value, const int alpha);
        static bool compareSmall(const int matrix_value, const int alpha);
        static bool compareEqual(const int matrix_value, const int alpha);
        static bool compareNotEqual(const int matrix_value, const int alpha);

    public:
        //constructor, return a matrix initilized with the given value, zeros as defualt
        IntMatrix(Dimensions dims, int initial_value = 0);
        //return a copy of the given matrix, copy each cell
        IntMatrix(const IntMatrix &matrix);
        //define the = opertor
        IntMatrix &operator=(const IntMatrix &matrix);
        //distructor
        ~IntMatrix();
        //return an identity marix in size of dimxdim
        static IntMatrix Identity( int dim);
        //return the number of lines in the matrix
        int height() const;
        //return the number of cloums in the matrix
        int width() const;
        //return the number of integers in the matrix
        int size() const;
        //return a transposed copy of the matrix
        IntMatrix transpose() const;
        //return a negative copy of the matrix
        IntMatrix operator-() const;
        //gets a scalar and add it to every argument of the matrix return a refernce to the matrix
        IntMatrix &operator+=(const int alpha);
        friend std::ostream &operator<<(std::ostream &os, const IntMatrix &matrix);
        //difines a semetric sum of two matrices return a copy of the sum
        friend IntMatrix operator+(const IntMatrix matrix_a, const IntMatrix matrix_b);
        //get a row and a colum and return a pointer to the specified cell
        int &operator()(const int i, const int j);
        //get a row and a coulom and return the value of the specified cell
        const int operator()(const int i, const int j) const;
        //get a scalar and retrun a matrix with 0 where the value were bigger and 1 where smaller
        IntMatrix operator<(const int alpha) const;
        //get a scalar and retrun a matrix with 0 where the value were bigger and 1 where smaller or equal
        IntMatrix operator<=(const int alpha) const;
        //get a scalar and retrun a matrix with 0 where the value were smaller and 1 where bigger
        IntMatrix operator>(const int alpha) const;
        //get a scalar and retrun a matrix with 0 where the value were smaller and 1 where bigger or equal
        IntMatrix operator>=(const int alpha) const;
        //get a scalar and retrun a matrix with 0 where the value were different and 1 where equal
        IntMatrix operator==(const int alpha) const;
        //get a scalar and retrun a matrix with 0 where the value were equal and 1 where differeny
        IntMatrix operator!=(const int alpha) const;
        //makes iterator class a member of the IntMatrix class
        class iterator;
        //return an iterator object that points to the first cell of the matrix
        IntMatrix::iterator begin();
        //return an itetaror object that points to the last cell of the matrix
        IntMatrix::iterator end();
        //make const_iterator class a member of the IntMatrix class
        class const_iterator;
        IntMatrix::const_iterator begin() const;
        IntMatrix::const_iterator end() const;
    };
    //define a sum of matrix + scalar:by adding and the scalar to to each cell of the matrix
    IntMatrix operator+(const IntMatrix matrix, const int alpha);
    //define a sum of scalar + matrix:by adding and the scalar to to each cell of the matrix
    IntMatrix operator+(const int alpha, const IntMatrix matrix);
    //define subtraction between two matrices return a copy of the result
    IntMatrix operator-(const IntMatrix matrix_a, const IntMatrix matrix_b);
    std::ostream &operator<<(std::ostream &os, const IntMatrix &matrix);
    //return true if all the matrix args are different that 0 otherwise flase
    bool all(const IntMatrix matrix);
    //return true if one of the matrix args are different then 0 otherwise flase
    bool any(const IntMatrix matrix);
    //implimention of iterator class
    class IntMatrix::iterator
    {
        IntMatrix *matrix; //the object the itartor points on
        int index;         //the current index that the iterator points to
        //constrator
        iterator(IntMatrix *matrix, int index);
        //in order to accsess IntMatrix members
        friend class IntMatrix;

    public:
        //return the value of the current place the iterator points to
        int &operator*() const;
        //progress the iterator to point on the next object and retrun the iterator
        iterator &operator++();
        //progress the iterator to point on the next object but return the same iterator before progression
        iterator operator++(int);
        //return true if the iterators points to the same place otherwise return flase
        bool operator==(const iterator &it) const;
        //return false if the iterators points to the same place otherwise return true
        bool operator!=(const iterator &it) const;
        //using default copy constractor
        iterator(const iterator &) = default;
        //usinf default operator of =
        iterator &operator=(const iterator &) = default;
    };
    //implimention of const iterator class
    class IntMatrix::const_iterator
    {
        const IntMatrix *matrix;                            //points on a read only matrix
        int index;                                          //the current place the iterator points to
        const_iterator(const IntMatrix *matrix, int index); //constractor
        friend class IntMatrix;                             //gain access to IntMatrix members

    public:
        //return the value the iterator points to
        const int &operator*() const;
        //return an iterator pointing to the next value
        const_iterator &operator++();
        //return an iterator pointing to the same value as before but progress it at the end of the line
        const_iterator operator++(int);
        //return true if the iterators points to the same places in the matrices otherwise false
        bool operator==(const const_iterator &it) const;
        //return false if the iterators points to the same places in the matrices otherwise true
        bool operator!=(const const_iterator &it) const;
        //usinf the difault copy constractor
        const_iterator(const const_iterator &) = default;
        //using the difualt = operator
        const_iterator &operator=(const const_iterator &) = default;
    };

} // namespace mtm
#endif //EX3_INTMATRIX