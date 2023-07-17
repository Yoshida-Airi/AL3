#include <WorldTransform.h>
#include"MathUtility.h"

void WorldTransform::UpdateMatrix() 
{
	matWorld_ = MakeAffinMatrix(scale_, rotation_, translation_);
	TransferMatrix();
}


