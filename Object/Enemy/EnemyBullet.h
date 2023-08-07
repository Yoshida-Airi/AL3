#pragma once
#include "MathUtility.h"
#include <Model.h>
#include <WorldTransform.h>
#include"player.h"

#include"Collider.h"
#include"CollisionConfig.h"

/// <summary>
/// 敵の弾
/// </summary>
class EnemyBullet : public Collider
{
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~EnemyBullet();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">敵の弾</param>
	/// <param name="position">弾の位置</param>
	/// <param name="velocity">弾の速度</param>
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(ViewProjection viewProjection);

	/// <summary>
	/// 衝突を検出したら呼び出されるコールバック関数
	/// </summary>
	void OnCollision();

	void SetPlayer(Player* player) { player_ = player; };

	bool IsDead() const { return isDead_; };

	// ワールド座標を取得
	Vector3 GetWorldPosition() override;

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// 速度
	Vector3 velocity_ = {};
	//自キャラ
	Player* player_ = nullptr;

	// 寿命<frm>
	static const int32_t kLifeTime = 60 * 5;
	// デスタイマー
	int32_t deathTimer_ = kLifeTime;
	// デスフラグ
	bool isDead_ = false;

};