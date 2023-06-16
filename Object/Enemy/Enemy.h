#pragma once
#include "Model.h"
#include "WorldAffinMatrix.h"
#include <cassert>
#include"EnemyBullet.h"
#include<list>
#include "TimedCall.h"

class Enemy;	//前方宣言


class BaseEnemyState
{
protected:
	Enemy* enemy_ = {0};

public:

	 // 仮想デストラクタ
	virtual ~BaseEnemyState(){};

	//純粋仮想関数
	virtual void update(Enemy* pEnemy, Vector3& velocity) = 0;

};

//接近
class EnemyStateApproach : public BaseEnemyState
{
public:
	void update(Enemy* pEnemy, Vector3& velocity);
};

//離脱
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


	void Initialize(Model* model, const Vector3& position);
	void Update();
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// ステート内容変更
	/// </summary>
	void ChangeState(BaseEnemyState* newState);

	/// <summary>
	/// 接近フェーズの初期化
	/// </summary>
	void ApproachInitialize();

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

	/// <summary>
	/// 弾を発射し、タイマーをリセットするコールバック関数
	/// </summary>
	void AttackReset();

public:	//静的メンバ変数
	//発射間隔
	static const int kFireInterval = 30;

private:	//メンバ変数
	// ワールド変換データ
	WorldTransform worldTransform_;
	WorldTransformEX AffinMatrix_;
	WorldTransformEX transform;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// 速度
	Vector3 ApprochVelocity_;
	Vector3 LeaveVelocity_;
	//ステート
	BaseEnemyState* state;
	// 弾
	std::list<EnemyBullet*> bullets_;
	//発射タイマー
	int32_t timer = 0;
	//時限発動のリスト
	std::list<TimedCall*> timedCalls_;

	bool isAttacEvent = false;
	

private:	//プライベートメソッド


	/// <summary>
	/// 弾発射
	/// </summary>
	void Fire();

	
};




