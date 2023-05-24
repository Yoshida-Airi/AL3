#pragma once
#include "Model.h"
#include "WorldAffinMatrix.h"
#include <cassert>
#include"WorldAffinMatrix.h"

class Enemy 
{
public:
	void Initialize(
	    Model* model, const Vector3& position, const Vector3& velocityA, const Vector3& velocityB);
	void Update();
	void Draw(const ViewProjection& viewProjection);

	enum class Phase {
		Approach, // 接近する
		Leave,    // 離脱する
	};

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	WorldTransformEX AffinMatrix_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// 速度
	Vector3 ApprochVelocity_;
	Vector3 LeaveVelocity_;
	//フェーズ
	Phase phase_ = {};

private:
	/// <summary>
	/// 接近フェーズの更新関数
	/// </summary>
	void Approach();

	/// <summary>
	/// 離脱フェーズの更新関数
	/// </summary>
	void Leave();

};
