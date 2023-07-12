#pragma once
#include "EnemyState.h"
#include "Model.h"
#include "WorldAffinMatrix.h"
#include <cassert>

class Enemy {
public:
	// コンストラクタ
	Enemy();
	// デストラクタ
	~Enemy();

	void Initialize(Model* model, const Vector3& position);
	void Update();
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// ステート内容変更
	/// </summary>
	void ChangeState(BaseEnemyState* newState);

	/// <summary>
	/// 移動
	/// </summary>
	void Move(Vector3& velocity);

	/// <summary>
	/// ゲッター
	/// </summary>
	Vector3 GetWorldTrans() { return worldTransform_.translation_; };

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	WorldTransformEX AffinMatrix_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// ステート
	BaseEnemyState* state = nullptr;

private:

	
	// メンバ関数ポインタのテーブル
	static void (Enemy::*spFuncTable[])();

};
