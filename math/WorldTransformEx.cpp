#include <WorldTransform.h>
#include"MathUtility.h"

void WorldTransform::UpdateMatrix() 
{
	matWorld_ = MakeAffinMatrix(scale_, rotation_, translation_);

	//親があれば親のワールド行列を掛ける
	if (parent_)
	{
		matWorld_ = Multiply(matWorld_, parent_->matWorld_);
	}

	TransferMatrix();
}


