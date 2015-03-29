/*
Amju Games source code (c) Copyright Jason Colman 2000-2008
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <assert.h>
#include "BoneWeight.h"

BoneWeight::BoneWeight()
{
  m_numBones = 0;
}

int BoneWeight::GetNumBones() const
{
  return m_numBones;
}

int BoneWeight::GetBoneIndex(int boneNum) const
{
  // boneNum should be 0..number of bones - 1
  assert(boneNum >= 0);
  assert(boneNum < MAX_NUM_BONE_WEIGHTS);
  return m_bw[boneNum].first;
}

// Weight should be between 0 and 1.0 ???
float BoneWeight::GetWeight(int boneNum) const
{
  return m_bw[boneNum].second;
}

void BoneWeight::AddBoneWeight(int boneIndex, float weight)
{
  assert(m_numBones < MAX_NUM_BONE_WEIGHTS);
  m_bw[m_numBones] = std::make_pair(boneIndex, weight);
  m_numBones++;
}




