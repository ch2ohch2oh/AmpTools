#ifndef COMPLEX_MATRIX_H
#define COMPLEX_MATRIX_H

#include "TComplex.h"

#include <iostream>

using namespace std;

class ComplexMatrix
{
public:
    ComplexMatrix(int nrows, int ncols);
    // Copy constructor
    ComplexMatrix(const ComplexMatrix & mat);
    ~ComplexMatrix();
    
    // Copy assignment
    ComplexMatrix & operator=(const ComplexMatrix & mat);

    bool operator==(const ComplexMatrix & mat) const;
    bool operator!=(const ComplexMatrix & mat) const;
    ComplexMatrix operator+(const ComplexMatrix & mat) const;
    ComplexMatrix operator-(const ComplexMatrix & mat) const;
    ComplexMatrix operator*(const ComplexMatrix & mat) const;
    ComplexMatrix operator*(const TComplex scalar) const;
    ComplexMatrix operator/(const TComplex scalar) const;
    friend ComplexMatrix operator*(const TComplex scalar, const ComplexMatrix & mat);
    TComplex get(int row, int col) const;
    ComplexMatrix & set(int row, int col, TComplex value);

    bool isSameShape(const ComplexMatrix & mat) const;

    friend ostream & operator<<(ostream & out, const ComplexMatrix & mat);

private:
    int m_rows;
    int m_cols;
    TComplex ** m_mat;
};

#endif
