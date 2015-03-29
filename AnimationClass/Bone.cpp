/*
Amju Games source code (c) Copyright Jason Colman 2000-2008
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Bone.h"
#include "File.h"
#ifdef WIN32
#include <GL/glut.h> 
#endif
#ifdef MACOSX
#include <GLUT/glut.h>
#endif

bool Vec3::Load(File* pf)
{
  if (!pf->GetFloat(&m_x))
  {
    pf->ReportError("Vec3: expected x");
    return false;
  }
  if (!pf->GetFloat(&m_y))
  {
    pf->ReportError("Vec3: expected y");
    return false;
  }
  if (!pf->GetFloat(&m_z))
  {
    pf->ReportError("Vec3: expected z");
    return false;
  }
  return true;
}

Bone::Bone()
{
  m_transform.SetIdentity();
  m_combined.SetIdentity();
}

#ifdef _DEBUG
void DrawLine(const Vec3& v1, const Vec3& v2)
{
  // Disable texturing while drawing line
  glDisable(GL_TEXTURE_2D);
  glBegin(GL_LINES);
  glVertex3f(v1.m_x, v1.m_y, v1.m_z);
  glVertex3f(v2.m_x, v2.m_y, v2.m_z);
  glEnd();
  glEnable(GL_TEXTURE_2D);
}

void Bone::Draw(Bone* pParent, int depth) const
{
  if (pParent)
  {
    // Draw bone using Combined Transforms
    Vec3 v1(pParent->m_combined.at(12), pParent->m_combined.at(13), pParent->m_combined.at(14));
    Vec3 v2(m_combined.at(12), m_combined.at(13), m_combined.at(14));

    DrawLine(v1, v2);
  }

  // Draw children
  for (unsigned int i = 0; i < m_children.size(); i++)
  {
    m_children[i]->Draw(const_cast<Bone*>(this), depth + 1); // pass this as parent
  }
}
#endif // _DEBUG

void Bone::AddChild(Bone* child)
{
  m_children.push_back(child);
}

void Bone::SetTransform(const Matrix& m)
{
  m_transform = m;
}

const Matrix& Bone::GetCombinedTransform() const
{
  return m_combined;
}

void Bone::CombineTransforms(Bone* parent)
{
  if (parent)
  {
    m_combined = m_transform * parent->m_combined;
  }
  else
  {
    m_combined = m_transform;
  }

  // Update children
  for (unsigned int i = 0; i < m_children.size(); i++)
  {
    m_children[i]->CombineTransforms(this);
  }
}


