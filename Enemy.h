#pragma once
#include "Model.h"
#include "WorldAffinMatrix.h"
#include <cassert>

class Enemy;	//前方宣言

class BaseEnemyState
{
public:
	Enemy* enemy_ = {0};
	Vector3& velocity;
	//純粋仮想関数
	virtual void update(Enemy* pEnemy, Vector3& velocity) = 0;
};

class EnemyStateApproach : public BaseEnemyState
{
public:
	void update(Enemy* pEnemy, Vector3& velocity);
};

class EnemyStateLeave : public BaseEnemyState
{
public:
	void update(Enemy* pEnemy, Vector3& velocity);
};


class Enemy 
{
public:

	//コンストラクタ
	Enemy();
	//デストラクタ
	~Enemy();


	void Initialize(
	    Model* model, const Vector3& position, const Vector3& velocityA, const Vector3& velocityB);
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
	/// 
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
	
	//メンバ関数ポインタのテーブル
	static void (Enemy::*spFuncTable[])();

};




