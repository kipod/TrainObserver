#ifndef XP_MATH_HPP
#define XP_MATH_HPP

// This file contains some cross platform math related stuff.

#include <math.h>
#include <xmmintrin.h>

#define EXT_MATH 1
//#define SSE_MATH3 1

#if EXT_MATH

#include "d3dx9math.h"

typedef D3DXMATRIX MatrixBase;
typedef D3DXQUATERNION QuaternionBase;
typedef D3DXVECTOR2 Vector2Base;
typedef D3DXVECTOR3 Vector3Base;
typedef D3DXVECTOR4 Vector4Base;

#define XPVec2Length D3DXVec2Length
#define XPVec2LengthSq D3DXVec2LengthSq
#define XPVec2Normalize D3DXVec2Normalize
#define XPVec2Dot D3DXVec2Dot
#define XPVec3Length D3DXVec3Length
#define XPVec3LengthSq D3DXVec3LengthSq
#define XPVec3Dot D3DXVec3Dot
#define XPVec3Cross D3DXVec3Cross
#define XPVec3Normalize D3DXVec3Normalize
#define XPVec3Lerp D3DXVec3Lerp
#define XPVec3Transform D3DXVec3Transform
#define XPVec3TransformCoord D3DXVec3TransformCoord
#define XPVec3TransformNormal D3DXVec3TransformNormal
#define XPVec4Transform D3DXVec4Transform
#define XPVec4Length D3DXVec4Length
#define XPVec4LengthSq D3DXVec4LengthSq
#define XPVec4Normalize D3DXVec4Normalize
#define XPVec4Lerp D3DXVec4Lerp
#define XPVec4Dot D3DXVec4Dot
#define XPMatrixIdentity D3DXMatrixIdentity
#define XPMatrixInverse D3DXMatrixInverse
#define XPMatrixRotationQuaternion D3DXMatrixRotationQuaternion
#define XPMatrixTranspose D3DXMatrixTranspose
#define XPMatrixfDeterminant D3DXMatrixDeterminant
#define XPMatrixOrthoLH D3DXMatrixOrthoLH
#define XPMatrixLookAtLH D3DXMatrixLookAtLH
#define XPMatrixOrthoOffCenterLH D3DXMatrixOrthoOffCenterLH
#define XPMatrixMultiply D3DXMatrixMultiply
#define XPMatrixPerspectiveFovLH D3DXMatrixPerspectiveFovLH
#define XPMatrixRotationX D3DXMatrixRotationX
#define XPMatrixRotationY D3DXMatrixRotationY
#define XPMatrixRotationZ D3DXMatrixRotationZ
#define XPMatrixScaling D3DXMatrixScaling
#define XPMatrixTranslation D3DXMatrixTranslation
#define XPMatrixRotationYawPitchRoll D3DXMatrixRotationYawPitchRoll
#define XPQuaternionDot D3DXQuaternionDot
#define XPQuaternionNormalize D3DXQuaternionNormalize
#define XPQuaternionRotationMatrix D3DXQuaternionRotationMatrix
#define XPQuaternionRotationYawPitchRoll D3DXQuaternionRotationYawPitchRoll
#define XPQuaternionSlerp D3DXQuaternionSlerp
#define XPQuaternionRotationAxis D3DXQuaternionRotationAxis
#define XPQuaternionMultiply D3DXQuaternionMultiply
#define XPQuaternionInverse D3DXQuaternionInverse
#define XPMatrixRotationAxis  D3DXMatrixRotationAxis

#endif

#endif
