#pragma once
#include "DebugCamera.h"
#include "Input.h"
#include "MathUtility.h"
#include "Model.h"
#include "PlayerBullet.h"

#include"Collider.h"
#include"CollisionConfig.h"

#include <list>
/// <summary>
/// 自キャラ
/// </summary>
class Player : public Collider
{
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialize(Model* model, uint32_t textureHandle);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection"ビュープロジェクション（参照渡し）</param>
	void Draw(ViewProjection viewProjection);

	/// <summary>
	/// 旋回
	/// </summary>
	void Rotate();

	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	/// <returns>プレイヤーのワールド座標</returns>
	Vector3 GetWorldPosition() override;

	/// <summary>
	/// 衝突を検出したら呼び出されるコールバック関数
	/// </summary>
	void OnCollision() override;

	//弾リストを取得
	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; };

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// キーボード入力
	Input* input_ = nullptr;
	// ImGuiで値を入力する変数
	float inputFloat3[3] = {0, 0, 0};
	// 弾
	std::list<PlayerBullet*> bullets_;

	// 攻撃
	void Attack();
};