#ifndef VEC3_H
#define VEC3_H

class File;

struct Vec3
{
  Vec3(float x = 0, float y = 0, float z = 0) : m_x(x), m_y(y), m_z(z) { } 
  float m_x, m_y, m_z;

  bool Load(File*);
};

inline Vec3 operator+(const Vec3& w, const Vec3& v)
{
  return Vec3(w.m_x + v.m_x, w.m_y + v.m_y, w.m_z + v.m_z);
}

inline Vec3 operator-(const Vec3& w, const Vec3& v)
{
  return Vec3(w.m_x - v.m_x, w.m_y - v.m_y, w.m_z - v.m_z);
}

inline Vec3 operator*(const Vec3& v, float f)
{
  return Vec3(v.m_x * f, v.m_y * f, v.m_z * f);
}

#endif
