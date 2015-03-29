/*
Amju Games source code (c) Copyright Jason Colman 2000-2008
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <math.h>
#include "KeyFrame.h"
#include "File.h"

KeyFrame::KeyFrame() : m_time(0)
{
}

float KeyFrame::GetTime() const
{
  return m_time;
}

const Matrix& KeyFrame::GetMatrix() const
{
  return m_matrix;
}

Vec3 Lerp(const Vec3& v1, const Vec3& v2, float t)
{
    return v1 + (v2 - v1)* t;
}

Matrix KeyFrame::Interpolate(const RST& r1, const RST& r2, float t)
{
  //Vec3 lerpScale = Lerp(r1.m_scale, r2.m_scale, t);

  Vec3 lerpTranslate = Lerp(r1.m_translate, r2.m_translate, t);

  Quaternion slerpRotate = Slerp(r1.m_quat, r2.m_quat, t);
  slerpRotate.Normalize();

  Matrix R, S, T;
  slerpRotate.CreateMatrix(&R);
  //S.Scale(lerpScale.m_x, lerpScale.m_y, lerpScale.m_z);
  T.Translate(lerpTranslate.m_x, lerpTranslate.m_y, lerpTranslate.m_z);

  //return R * S * T;
  return R * T; // no scale for now
}

RST CreateRST(const Matrix& mat)
{
  RST rst;
  rst.m_translate = Vec3(mat.at(12), mat.at(13), mat.at(14));
  rst.m_quat.CreateFromMatrix(mat);

  // Check that Quat is unit length
  //Assert(fabs(rst.m_quat.SquaredLength() - 1.0f) < 0.0001f);
  
  rst.m_quat.Normalize();

  // TODO No scale for now
  rst.m_scale = Vec3(1, 1, 1);
  return rst;
}

Matrix KeyFrame::Interpolate(
  PKeyFrame k1, PKeyFrame k2, float between)
{
  // TODO TEMP TEST - enable for no interpolation
  //return k1->m_matrix;

  // Create RST for each matrix, then interp.
  // - we can't directly interp 2 matrices.
  RST rst1 = CreateRST(k1->m_matrix);
  RST rst2 = CreateRST(k2->m_matrix);

  // Get a matrix of the interpolated RSTs
  return Interpolate(rst1, rst2, between);
}

bool KeyFrame::Load(File* pf)
{
  if (!pf->GetFloat(&m_time))
  {
    pf->ReportError("Expected keyframe time");
    return false;
  }
  if (m_time < 0)
  {
    pf->ReportError("Bad time for keyframe");
    return false;
  }

  return m_matrix.Load(pf);
}



