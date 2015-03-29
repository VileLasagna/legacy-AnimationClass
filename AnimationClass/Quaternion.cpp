/*
Amju Games source code (c) Copyright Jason Colman 2005
*/

#ifdef WIN32
#define _USE_MATH_DEFINES
#endif
#include <math.h>

#ifdef WIN32
#pragma warning(disable: 4786)
#endif
#include "Quaternion.h"
#include "Matrix.h"

Quaternion::Quaternion()
{
    m_x = m_y = m_z = 0.0f;
    m_w = 1.0f;
}

Quaternion::~Quaternion()
{
}

float Quaternion::SquaredLength() const
{
  return m_x * m_x + m_y * m_y + m_z * m_z + m_w * m_w;
}

void Quaternion::Normalize()
{
  float n = 1.0f / sqrt(SquaredLength());
  m_w *= n;
  m_x *= n;
  m_y *= n;
  m_z *= n;
}

void Quaternion::CreateFromAxisAngle(float x, float y, float z, float degrees)
{
    // First we want to convert the degrees to radians 
    // since the angle is assumed to be in radians
    float angle = float((degrees / 180.0f) * M_PI);
    // Here we calculate the sin( theta / 2) once for optimization
    float result = (float)sin( angle / 2.0f );
		
    // Calcualte the w value by cos( theta / 2 )
    m_w = (float)cos( angle / 2.0f );
    // Calculate the x, y and z of the quaternion
    m_x = float(x * result);
    m_y = float(y * result);
    m_z = float(z * result);
}

void Quaternion::CreateFromMatrix(const Matrix& m)
{
  float trace = m.at(0) + m.at(5) + m.at(10) + 1.0f;
  static const float EPSILON = 0.0001f;
  if( trace > EPSILON ) 
  {
    float s = 0.5f / sqrtf(trace);
    m_w = 0.25f / s;
    m_x = (m.at(6) - m.at(9)) * s;
    m_y = (m.at(8) - m.at(2)) * s;
    m_z = (m.at(1) - m.at(4)) * s;
  } 
  else 
  {
    if ( m.at(0) > m.at(5) && m.at(0) > m.at(10) ) 
	{
      float s = 2.0f * sqrtf( 1.0f + m.at(0) - m.at(5) - m.at(0));
      m_w = (m.at(9) - m.at(6)) / s;
      m_x = 0.25f * s;
      m_y = (m.at(4) + m.at(1)) / s;
      m_z = (m.at(8) + m.at(2)) / s;
    } 
	else if (m.at(5) > m.at(10)) 
	{
      float s = 2.0f * sqrtf( 1.0f + m.at(5) - m.at(0) - m.at(10));
      m_w = (m.at(8) - m.at(2)) / s;
      m_x = (m.at(4) + m.at(1)) / s;
      m_y = 0.25f * s;
      m_z = (m.at(9) + m.at(6)) / s;
    } 
	else 
	{
      float s = 2.0f * sqrtf( 1.0f + m.at(10) - m.at(0) - m.at(5));
      m_w = (m.at(4) - m.at(1)) / s;
      m_x = (m.at(8) + m.at(2)) / s;
      m_y = (m.at(9) + m.at(6)) / s;
      m_z = 0.25f * s;
    }
  }
}

void Quaternion::CreateMatrix(Matrix *pMatrix) const
{
  CreateMatrix(pMatrix->m_e); // OK - we are a friend
}

void Quaternion::CreateMatrix(float *pMatrix) const
{
    // Make sure the matrix has allocated memory to store the rotation data
    if(!pMatrix) return;

	pMatrix[ 0] = 1.0f - 2.0f * ( m_y * m_y + m_z * m_z ); 
    pMatrix[ 1] = 2.0f * (m_x * m_y + m_z * m_w);
    pMatrix[ 2] = 2.0f * (m_x * m_z - m_y * m_w);
    pMatrix[ 3] = 0.0f;  

	pMatrix[ 4] = 2.0f * ( m_x * m_y - m_z * m_w );  
    pMatrix[ 5] = 1.0f - 2.0f * ( m_x * m_x + m_z * m_z ); 
    pMatrix[ 6] = 2.0f * (m_z * m_y + m_x * m_w );  
    pMatrix[ 7] = 0.0f;  

	pMatrix[ 8] = 2.0f * ( m_x * m_z + m_y * m_w );
    pMatrix[ 9] = 2.0f * ( m_y * m_z - m_x * m_w );
    pMatrix[10] = 1.0f - 2.0f * ( m_x * m_x + m_y * m_y );  
    pMatrix[11] = 0.0f;  

	pMatrix[12] = 0;  
    pMatrix[13] = 0;  
    pMatrix[14] = 0;  
    pMatrix[15] = 1.0f;
}

Quaternion Quaternion::operator *(Quaternion q)
{
    Quaternion r;
    r.m_w = m_w*q.m_w - m_x*q.m_x - m_y*q.m_y - m_z*q.m_z;
    r.m_x = m_w*q.m_x + m_x*q.m_w + m_y*q.m_z - m_z*q.m_y;
    r.m_y = m_w*q.m_y + m_y*q.m_w + m_z*q.m_x - m_x*q.m_z;
    r.m_z = m_w*q.m_z + m_z*q.m_w + m_x*q.m_y - m_y*q.m_x;
	
    return(r);
}

Quaternion Slerp(
  const Quaternion& from, const Quaternion& to, float t)
{
  float to1[4];
  double omega, cosom, sinom, scale0, scale1;

  // calc cosine
  cosom = from.m_x * to.m_x + from.m_y * to.m_y + from.m_z * to.m_z
			       + from.m_w * to.m_w;

  // adjust signs (if necessary)
  if (cosom < 0)
  { 
    cosom = -cosom; 
    to1[0] = - to.m_x;
    to1[1] = - to.m_y;
    to1[2] = - to.m_z;
    to1[3] = - to.m_w;
  } 
  else  
  {
    to1[0] = to.m_x;
    to1[1] = to.m_y;
    to1[2] = to.m_z;
    to1[3] = to.m_w;
  }

  // calculate coefficients

  static const double DELTA = 0.001;
  if ( (1.0 - cosom) > DELTA ) 
  {
    // standard case (slerp)
    omega = acos(cosom);
    sinom = sin(omega);
    scale0 = sin((1.0 - t) * omega) / sinom;
    scale1 = sin(t * omega) / sinom;
  } 
  else 
  {        
    // "from" and "to" quaternions are very close 
    //  ... so we can do a linear interpolation
    scale0 = 1.0 - t;
    scale1 = t;
  }

  Quaternion res;

  // calculate final values
  res.m_x = (float)(scale0 * from.m_x + scale1 * to1[0]);
  res.m_y = (float)(scale0 * from.m_y + scale1 * to1[1]);
  res.m_z = (float)(scale0 * from.m_z + scale1 * to1[2]);
  res.m_w = (float)(scale0 * from.m_w + scale1 * to1[3]);

  return res;
}

