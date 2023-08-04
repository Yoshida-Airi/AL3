#pragma once
#include "Model.h"
#include <cassert>

#include"MathUtility.h"
#include"IEnemyState.h"

#include"EnemyStateApproach.h"

class Enemy 
{
public:

	~Enemy();

	void Initialize(Model* model, const Vector3& position);
	void Update();
	void Draw(const ViewProjection& viewProjection);

	void Move(Vector3& velocity);

	/// <summary>
	/// ゲッター
	/// </summary>
	Vector3 GetWorldPosition() const { return worldTransform_.translation_; };

	// フェーズの以降
	void ChangeState(BaseEnemyState* newState);

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// 速度
	Vector3 ApprochVelocity_;
	Vector3 LeaveVelocity_;

	// ステート
	BaseEnemyState* state;

private:
	

	//メンバ関数ポインタのテーブル
	static void (Enemy::*spFuncTable[])();

};
