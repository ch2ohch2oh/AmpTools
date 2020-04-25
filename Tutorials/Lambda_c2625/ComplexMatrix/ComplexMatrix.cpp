#include <iostream>
#include <stdexcept>
#include <limits>

#include "ComplexMatrix.h"

ComplexMatrix::ComplexMatrix(int nrows, int ncols)
{
    // cout << "Constructor called" << endl;
    m_rows = nrows;
    m_cols = ncols;
    m_mat = new TComplex * [m_rows];
    if(!m_mat) {
        throw runtime_error("Fail to allocate mem!");
    }
    for(int i = 0; i < m_rows; i++) {
        m_mat[i] = new TComplex[m_cols];
        if(!m_mat[i]) {
            throw runtime_error("Failed to allocate mem!");
        }
        for(int j = 0; j < m_cols; j++) {
            m_mat[i][j] = TComplex(0);
        }
    }
}

ComplexMatrix::~ComplexMatrix()
{
    for(int i = 0; i < m_rows; i++) {
        delete [] m_mat[i];
    }
    delete [] m_mat;
}

// To call constructor within a constructor, you have to
// use initialization list
ComplexMatrix::ComplexMatrix(const ComplexMatrix & mat) :
    ComplexMatrix(mat.m_rows, mat.m_cols)
{
    // cout << "Copy constructor called" << endl;

    for(int i = 0; i < m_rows; i++) {
        for(int j = 0; j < m_cols; j++) {
            m_mat[i][j] = mat.m_mat[i][j];
        }
    }
}

ComplexMatrix & ComplexMatrix::operator=(const ComplexMatrix & mat)
{
    // cout << "Assignment constructor called" << endl;

    if(this == &mat) {
        return *this;
    }
    if(mat.m_rows != m_rows || mat.m_cols != m_cols) {
        throw runtime_error("Destination matrix has different shape!");
    }
    for(int i = 0; i < m_rows; i++) {
        for(int j = 0; j < m_cols; j++) {
            m_mat[i][j] = mat.m_mat[i][j];
        }
    }
    return *this;
}

bool ComplexMatrix::operator==(const ComplexMatrix & mat) const
{
    if(this == &mat) {
        return true;
    }
    if(m_rows != mat.m_rows || m_cols != mat.m_cols) {
        throw runtime_error("Matrix shape does not match!");
    }
    for(int i = 0; i < m_rows; i++) {
        for(int j = 0; j < m_cols; j++) {
            if(TComplex::Abs(m_mat[i][j] - mat.m_mat[i][j]) > std::numeric_limits<double>::epsilon()) {
                return false;
            }
        }
    }
    return true;
}

bool ComplexMatrix::operator!=(const ComplexMatrix & mat) const 
{
    return !(*this == mat);
}

ComplexMatrix ComplexMatrix::operator+(const ComplexMatrix & mat) const 
{
    if(!isSameShape(mat)) {
        throw runtime_error("Cannot add two incompatible shapes!");
    }
    ComplexMatrix result(*this);
    for(int i = 0; i < m_rows; i++) {
        for(int j = 0; j < m_cols; j++) {
            result.m_mat[i][j] += mat.m_mat[i][j];
        }
    }
    return result;
}

ComplexMatrix ComplexMatrix::operator-(const ComplexMatrix & mat) const 
{
    if(!isSameShape(mat)) {
        throw runtime_error("Cannot subtract two incompatible shapes!");
    }
    ComplexMatrix result(*this);
    for(int i = 0; i < m_rows; i++) {
        for(int j = 0; j < m_cols; j++) {
            result.m_mat[i][j] -= mat.m_mat[i][j];
        }
    }
    return result;
}

ComplexMatrix ComplexMatrix::operator*(const ComplexMatrix & mat) const
{
    if(m_cols != mat.m_rows) {
        throw runtime_error("Cannot multiply incompatible shapes!");
    }
    ComplexMatrix result(m_rows, mat.m_cols);
    for(int i = 0;i < m_rows; i++) {
        for(int j = 0; j < mat.m_cols; j++) {
            for(int k = 0; k < m_cols; k++) {
                result.m_mat[i][j] += m_mat[i][k] * mat.m_mat[k][j];
            }
        }
    }
    return result;
}

ComplexMatrix ComplexMatrix::operator*(const TComplex scalar) const
{
    ComplexMatrix result(*this);
    for(int i = 0;i < m_rows; i++) {
        for(int j = 0; j < m_cols; j++) {
            result.m_mat[i][j] *= scalar;
        }
    }
    return result;
}

ComplexMatrix operator*(const TComplex scalar, const ComplexMatrix & mat)
{
    return mat * scalar;
}

ComplexMatrix ComplexMatrix::operator/(const TComplex scalar) const
{
    return *this * (TComplex(1, 0) / scalar);
}

TComplex ComplexMatrix::get(int row, int col) const
{
    if(!(row >= 0 && row < m_rows && col >=0 && col < m_cols)) {
        throw runtime_error("Index out of bound!");
    }
    return m_mat[row][col];
}

ComplexMatrix & ComplexMatrix::set(int row, int col, TComplex value)
{
    if(!(row >= 0 && row < m_rows && col >=0 && col < m_cols)) {
        throw runtime_error("Index out of bound!");
    }
    m_mat[row][col] = value;
    return *this;
}

bool ComplexMatrix::isSameShape(const ComplexMatrix & mat) const
{
    return mat.m_rows == m_rows && mat.m_cols == m_cols;
}

ostream & operator<<(ostream & out, const ComplexMatrix & mat)
{
    out << "Matrix shape = (" << mat.m_rows << " ," 
        << mat.m_cols << ")" << endl;
    for(int i = 0; i < mat.m_rows; i++) {
        for(int j = 0; j < mat.m_rows; j++) {
            out << mat.m_mat[i][j] << "\t";
        }
        out << endl;
    }
}