/*
Amju Games source code (c) Copyright Jason Colman 2000-2008
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Anim.h"
#include "File.h"

Anim::Anim()
{
  m_repeats = true; 
  m_animDuration = 4.0f;
}

bool Anim::Repeats() const
{
  return m_repeats;
}

float Anim::GetDuration() const
{
  return m_animDuration;
}

void Anim::SetSkeleton(Skeleton* pSkel)
{
  m_pSkel = pSkel;
}

bool Anim::Load(File* f)
{
  // TODO load duration

  int numBones = 0;
  if (!f->GetInteger(&numBones))
  {
    f->ReportError("Anim: expected num bones (joints)");
    return false;
  }

  for (int j = 0; j < numBones; j++)
  {
    std::string boneName;
    if (!f->GetString(&boneName))
    {
      f->ReportError("Expected bone name");
      return false;
    }

    // Look up ID for bone name
    int id = m_pSkel->GetIdForString(boneName);
    if (id < 0)
    {
      f->ReportError("Bad bone name " + boneName);
      return false;
    }

    int numKeyFrames = 0;
    if (!f->GetInteger(&numKeyFrames))
    {
      f->ReportError("Expected number of keyframes");
      return false;
    }

    KeyFrames kfs;

    // Load each key frame for the bone.
    for (int i = 0; i < numKeyFrames; i++)
    {
      PKeyFrame k = new KeyFrame;
      if (!k->Load(f))
      {
        f->ReportError("Failed to load keyframe");
        return false;
      }
      // Map time to keyframe, so we can quickly look up
      // Also orders keyframes by time - nice
      kfs[k->GetTime()] = k;

      if (k->GetTime() > m_animDuration)
      {
        m_animDuration = k->GetTime();
      }
    }
    m_boneKeyFrames[id] = kfs;
  }

  return true;
}

const BoneKeyFrames& Anim::GetBoneKeyFrames() const
{
  return m_boneKeyFrames;
}


