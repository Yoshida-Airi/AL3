#pragma once
#include "Model.h"
#include "WorldAffinMatrix.h"
#include <cassert>
#include"EnemyState.h"


class Enemy 
{
public:

	//コンストラクタ
	Enemy();
	//デストラクタ
	~Enemy();


	void Initialize(Model* model, const Vector3& position);
	void Update();
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// ステート内容変更
	/// </summary>
	void ChangeState(BaseEnemyState* newState);

	/// <summary>
	/// 接近フェーズの更新関数
	/// </summary>
	void Approach();

	/// <summary>
	/// 離脱フェーズの更新関数
	/// </summary>
	void Leave();

	/// <summary>
	/// 移動
	/// </summary>
	void Move(Vector3& velocity);

/// <summary>
/// ゲッター
/// </summary>
	Vector3 GetWorldTrans() const { return worldTransform_.translation_; };

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
	//ステート
	BaseEnemyState* state;

private:
	

};




