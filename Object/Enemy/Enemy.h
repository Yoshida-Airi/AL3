#pragma once
#include "MathUtility.h"
#include "Model.h"
#include "TimedCall.h"
#include <cassert>
#include <list>

class Enemy;  // 前方宣言
class Player; // 自機クラスの前方宣言
class GameScene;	//ゲームシーンの前方宣言

class BaseEnemyState {
protected:
	Enemy* enemy_ = {0};

public:
	// 仮想デストラクタ
	virtual ~BaseEnemyState(){};

	// 純粋仮想関数
	virtual void Initialize(Enemy* pEnemy) = 0;
	virtual void update(Enemy* pEnemy) = 0;
};

// 接近
class EnemyStateApproach : public BaseEnemyState {
public:
	void Initialize(Enemy* pEnemy);
	void update(Enemy* pEnemy);
};

// 離脱
class EnemyStateLeave : public BaseEnemyState {
public:
	void Initialize(Enemy* pEnemy);
	void update(Enemy* pEnemy);
};

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
	Vector3 GetWorldPosition() const { return worldTransform_.translation_; };

	/// <summary>
	/// 弾を発射し、タイマーをリセットするコールバック関数
	/// </summary>
	void AttackReset();

	// セッター
	void SetPlayer(Player* player);
	void SetGameScene(GameScene* gameScene);


		
	/// <summary>
	/// 衝突を検出したら呼び出されるコールバック関数
	/// </summary>
	void OnCollision();

	
	bool IsDead() const { return isDead_; };

public: // 静的メンバ変数
	// 発射間隔
	static const int kFireInterval = 30;

private: // メンバ変数
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

	// 発射タイマー
	int32_t timer = 0;
	// 時限発動のリスト
	std::list<TimedCall*> timedCalls_;

	// 自キャラ
	Player* player_ = nullptr;

	// ゲームシーン
	GameScene* gameScene_ = nullptr;

	bool isAttacEvent = false;
	bool isDead_ = false;

private: // プライベートメソッド
	

	/// <summary>
	/// 弾発射
	/// </summary>
	void Fire();
};
