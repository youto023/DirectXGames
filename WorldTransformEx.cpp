#include "WorldTransform.h"
#include"MathUtilityForText.h"
void WorldTransform::UpdeteMatrix() {
	matWorld_ = MakeAffineMatrix(scale_, rotation_, translation_); 

TransferMatrix();
}


