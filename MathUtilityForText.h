#pragma once

#include<math.h>
#include"Matrix4x4.h"
#include"Vector3.h"

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotation, const Vector3& translate);
