/*
Amju Games source code (c) Copyright Jason Colman 2004
*/

#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

#include "Vec3.h"

class File;

class Matrix
{
    friend class Quaternion;

public:
    Matrix();

    // Sets the matrix to the identity matrix.
    void SetIdentity();

    // Load the current OpenGL modelview matrix into this matrix.
    void ModelView();

    // Load the current OpenGL projection matrix into this matrix.
    void Projection();

    // These functions set the matrix values in order to perform the 
    // transformation.  
    void RotateX(float xradians);
    void RotateY(float yradians);
    void RotateZ(float zradians);
    void Translate(float x, float y, float z);
    void Scale(float x, float y, float z);

    // Matrix inversion.
    // Sets the matrix pointed to by pResult to be the inverse of the
    // current matrix, if this is possible. If successful, returns true.
    // (Inverse A' of matrix A is defined as
    //    A x A' = I (the identity matrix) and also A' x A = I.)
    bool Inverse(Matrix* pResult) const;

    friend Matrix operator*(const Matrix& m, const Matrix& n);
    Matrix& operator*=(const Matrix& n);

    friend Matrix operator-(const Matrix& m, const Matrix& n);

    friend Vec3 operator*(const Vec3& p, const Matrix& m);

    float at(int row, int col) const { return m_e[row * 4 + col]; }
 
    float at(int index) const { return m_e[index]; }

    void set(int row, int col, float t) { m_e[row * 4 + col] = t; } 

    bool operator==(const Matrix& rhs) const;

    bool Load(File*);

private:
    float m_e[16];
};

#endif
