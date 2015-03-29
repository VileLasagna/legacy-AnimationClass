/*
Amju Games source code (c) Copyright Jason Colman 2000-2008
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <assert.h>
#include "AnimPlayer.h"
#include "Skeleton.h"
#ifdef WIN32
#include <windows.h>
#include <GL/gl.h>
#else
// Mac
#include <OpenGL/gl.h>
#endif

AnimPlayer::AnimPlayer()
{
  m_time = 0;
  m_hasLooped = false;
}

float AnimPlayer::GetTime() const
{
  return m_time;
}

void AnimPlayer::ResetTime()
{
  m_time = 0;
}

void AnimPlayer::CreateBones(const Skeleton* skel)
{
  skel->CreateBoneArray(&m_bones);
  m_bones[0].CombineTransforms(0);
}

void AnimPlayer::SetMesh(PMesh p)
{
  m_pMesh = p;
}

void AnimPlayer::SetAnim(PAnim panim)
{
  // TODO Store old and new anims so we can interpolate between them

  // Current animation, so we know where to get the next frame
  m_pAnim = panim;

  // Presumably we always start at beginning when a new anim is set
  m_time = 0;

  m_hasLooped = false;
}

void AnimPlayer::Update(float dt, bool interpolate)
{
  if (!m_pAnim)
  {
    return;
  }

  // Need some bones!
  assert(!m_bones.empty()); 
  
  m_time += dt; 

  // Iterate over data in the Anim.
  // For each bone in the anim:
  //  - if no keyframes, do nothing - go to next bone: bone transform is just identity
  //  - if one keyframe, use the one and only transform - no interpolation
  //  - more than one keyframe, interpolate using current time
  // So we can set the current transform for each bone:
  //  the original transform (imported) * current transform 
  // Then do Combine Transforms
  const BoneKeyFrames& bks = m_pAnim->GetBoneKeyFrames();
  for (BoneKeyFrames::const_iterator it = bks.begin();
       it != bks.end();
       ++it)
  {
    unsigned int boneId = it->first;
    assert(boneId >= 0);
    assert(boneId < m_bones.size());
    Bone* pBone = &m_bones[boneId];

    const KeyFrames& kfs = it->second;

    Matrix interpTransform;
    if (kfs.empty())
    {
    }
    /*
    else if (kfs.size() == 1)
    {
      // Nothing to interpolate
      KeyFrames::const_iterator it = kfs.begin();
      PKeyFrame kf = it->second;
      interpTransform = kf->GetMatrix();

      pBone->SetTransform(interpTransform);

      // TODO If we have looped, this is correct. 
      // But if we have not looped, should we 
      //  interpolate from bind pose to this KF ?
      // What we should do is interpolate from the previous position 
      //  to the new position.. TODO
    }
    */
    else
    {
      // Interpolate
      // Get the two keyframes on either side of m_time.
      // Interpolate between them.

      // We need to know the length of the anim period, so if we loop round, 
      //  we know how long the duration is from the final keyframe back 
      //  to the first.
      // Look up the KeyFrame in kfs. Get the element whose time value is 
      //  greater than m_time, but the lowest. Use lower_bound

      KeyFrames::const_iterator it1 = kfs.lower_bound(m_time);

      // it1 now points to the next frame after m_time
      KeyFrames::const_iterator it0 = it1;

      bool wrap = false;

      // Make it0 point to the element before it1. But if that is before the start
      //  of the map, wrap round to the end.
      if (it0 == kfs.begin())
      {
        wrap = true;
        it0 = kfs.end();
      }
      --it0;

      // If it1 points beyond the final element, point to the first element.
      if (it1 == kfs.end())
      {
        wrap = true;
        it1 = kfs.begin();
      }

      float time0 = it0->first;
      float time1 = it1->first;

      // Get 'between' value [0..1]
      float between = 0;
      if (wrap)
      {
        // Wrap around
        float timeDiff = time1 + m_pAnim->GetDuration() - time0;
        if (m_time >= time0)
        {
          between = (m_time - time0) / timeDiff;
        }
        else
        {
          assert(m_time < time1);
          between = (m_time + m_pAnim->GetDuration() - time0) / timeDiff;
        }
      }
      else
      {
        float timeDiff = time1 - time0;
        between = (m_time - time0) / timeDiff;
      }

      PKeyFrame kf0 = it0->second;
      PKeyFrame kf1 = it1->second;
      if (interpolate)
      {
        interpTransform = KeyFrame::Interpolate(kf0, kf1, between);
      }
      else
      {
        interpTransform = kf1->GetMatrix();
      }

      pBone->SetTransform(interpTransform);
    }
  }

  // Now traverse the bone tree, combining transforms.
  // (param 0 means no parent - this is the root)
  m_bones[0].CombineTransforms(0); 

  // Finally, loop the timer if we have got to the end of the anim.
  // TODO Freeze/repeat flag
  if (m_time > m_pAnim->GetDuration())
  {
    m_time = 0; //1.0f/24.0f; // TODO should be 1/24 ??
    m_hasLooped = true;
  }
}

void AnimPlayer::DrawSkel()
{
  // Debug draw skeleton
#ifdef _DEBUG
  glPushAttrib(GL_LIGHTING_BIT);
  glDisable(GL_LIGHTING);
  glColor3f(0, 0, 1.0f);
  glLineWidth(3);
  m_bones[0].Draw(0, 0); // no parent, so 0; depth is 0
  glLineWidth(1);
  glColor3f(1, 1, 1);
  glPopAttrib();
#endif
}

void AnimPlayer::Draw()
{
  if (m_pMesh)
  {
    m_pMesh->Draw(&m_bones[0]);
  }
}

