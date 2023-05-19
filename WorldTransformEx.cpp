#include <WorldTransform.h>
#include"WorldAffinMatrix.h"

void WorldTransform::UpdateMatrix() 
{
	WorldTransformEX matrix;
	matWorld_ = matrix.MakeAffinMatrix(scale_, rotation_, translation_);
	TransferMatrix();
}

