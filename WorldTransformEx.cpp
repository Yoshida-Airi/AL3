#include <WorldTransform.h>
#include"WorldAffinMatrix.h"

void WorldTransform::UpdateMatrix() 
{
	matWorld_ = MakeAffinMatrix(scale_, rotation_, translation_);
	TransferMatrix();
}


