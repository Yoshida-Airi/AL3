#pragma once
#include "Model.h"
#include "WorldAffinMatrix.h"
#include <cassert>
#include"WorldAffinMatrix.h"

class Enemy 
{
public:
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);
	void Update();
	void Draw(const ViewProjection& viewProjection);

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	WorldTransformEX AffinMatrix_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// 速度
	Vector3 velocity_;

};
