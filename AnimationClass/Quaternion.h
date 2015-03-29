/*
Amju Games source code (c) Copyright Jason Colman 2005
*/

#ifndef AMJU_QUATERNION_H_INCLUDED
#define AMJU_QUATERNION_H_INCLUDED

class Matrix;
class File;

class Quaternion  
{
public:
  Quaternion();
  ~Quaternion();
  Quaternion operator*(Quaternion q);
  void CreateMatrix(Matrix* pMatrix) const;
  void CreateFromAxisAngle(float x, float y, float z, float degrees);
  void CreateFromMatrix(const Matrix& m);

  friend Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float between);

  float SquaredLength() const;

  void Normalize();

  bool Load(File*);

private:
  void CreateMatrix(float* pMatrix) const;

private:
  float m_w;
  float m_z;
  float m_y;
  float m_x;
};

#endif 
