#ifndef SKIN_H_INCLUDED
#define SKIN_H_INCLUDED

#include "BoneWeight.h"
#include "RCPtr.h"
#include "Skeleton.h"

// Data for an animated mesh, has the bone weightings for every vertex.
class Skin : public RefCounted
{
public:
  bool Load(File*);

  void SetSkeleton(Skeleton* pSkel);

  const BoneWeight& GetBoneweight(int vertIndex);

  const Matrix& GetInverseCombinedBindPoseTransform(int boneIndex);

private:
  BoneWeights m_boneweights;
  PSkeleton m_pSkel;
};

typedef RCPtr<Skin> PSkin;

#endif
