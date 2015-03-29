/*
Amju Games source code (c) Copyright Jason Colman 2000-2008
*/

#ifndef AMJU_INDEXED_MESH_H_INCLUDED
#define AMJU_INDEXED_MESH_H_INCLUDED

#include "RCPtr.h"
#include "Skin.h"

// Vertex: (x, y, z) abs coord, (u, v) texture coord and normal.
struct Vert
{
  Vert() {}
  Vert(float x, float y, float z, float u, float v, float nx, float ny, float nz) :
    m_x(x), m_y(y), m_z(z), m_nx(nx), m_ny(ny), m_nz(nz), m_u(u), m_v(v) {}

  // Order for D3D FVF
  float m_x, m_y, m_z, m_nx, m_ny, m_nz, m_u, m_v;
};
typedef std::vector<Vert> Verts;

// For non-indexed triangle list, this is simply the 3 verts of a triangle.
struct Tri
{
  Vert m_verts[3];
};

typedef std::vector<Tri> Tris;

struct IndexedTriList
{ 
  // List of tris. Each tri has 3 verts, 3 UVs and 3 normals (x, y, z)
  struct Tri 
  {
    int m_index[3]; // Each element is an index into an array of Verts
    int m_uvIndex[3]; // Each element is an index into an array of UVs
    Vec3 m_normals[3];
  };
  std::vector<Tri> m_indexes;
};

struct UV
{
  UV() {}
  UV(float u, float v) : m_u(u), m_v(v) {}
  float m_u, m_v;
};
typedef std::vector<UV> UVs;

class File;

class IndexedMesh : public RefCounted
{
public:
  bool Load(File*);

  // Draw, passing in transformed bone matrices. We use them and the Skin (bone
  //  weights) to transform each vertex.
  // If no bones are passed in, or there is no skin, the mesh is not transformed.
  void Draw(Bone* arrayOfBones = 0);  

  void SetSkin(Skin*);

private:
  // List of bones and weights
  PSkin m_pSkin;

  Verts m_verts;
  UVs m_uvs;
  IndexedTriList m_tris;
};

typedef RCPtr<IndexedMesh> PMesh;

#endif
