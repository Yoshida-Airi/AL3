#pragma once
#include"WorldTransform.h"
#include"Model.h"

#include<cassert>

class Ground 
{
public:
	void Initialize(Model* model, const Vector3& position);
	void Update();
	void Draw(ViewProjection viewProjection_);

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

};
