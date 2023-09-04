#pragma once
#include "DebugCamera.h"
#include "Input.h"
#include "MathUtility.h"
#include "Model.h"
#include "PlayerBullet.h"
#include"Sprite.h"

#include <list>
/// <summary>
/// 自キャラ
/// </summary>
class Player {
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
	void Initialize(Model* model, Model* bulletModel, Vector3 positon);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(const ViewProjection viewProjection_);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection"ビュープロジェクション（参照渡し）</param>
	void Draw(ViewProjection viewProjection);

	/// <summary>
	/// UI描画
	/// </summary>
	void DrawUI();

	/// <summary>
	/// 旋回
	/// </summary>
	void Rotate();

	/// <summary>
	/// 自機のワールド座標を取得
	/// </summary>
	/// <returns>プレイヤーのワールド座標</returns>
	Vector3 GetWorldPosition();

	/// <summary>
	/// レティクルのワールド座標を取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetReticleWorldPosition();

	/// <summary>
	/// 衝突を検出したら呼び出されるコールバック関数
	/// </summary>
	void OnCollision();

	//弾リストを取得
	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; };

	/// <summary>
	/// 親となるワールドトランスフォームをセット
	/// </summary>
	/// <param name="parent">親となるワールドトランスフォーム</param>
	void SetParent(const WorldTransform* parent);

	bool GetAlive() const { return Alive; };

	void Reset();
			

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// 3Dレティクル用ワールドトランスフォーム
	WorldTransform worldTransform3DReticle_;
	// モデル
	Model* model_ = nullptr;
	Model* bulletModel_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// キーボード入力
	Input* input_ = nullptr;
	// ImGuiで値を入力する変数
	float inputFloat3[3] = {0, 0, 0};
	// 弾
	std::list<PlayerBullet*> bullets_;
	
	//2Dレティクル用スプライト
	Sprite* sprite2DReticle_ = nullptr;
	Sprite* HPSprite1_ = {};
	Sprite* HPSprite2_ = {};
	Sprite* HPSprite3_ = {};

	int HP = 3;
	bool Alive = true;

private:

	// 攻撃
	void Attack();

	/// <summary>
	/// レティクルの設定
	/// </summary>
	/// <param name="viewProjection_"></param>
	void SettingReticle(const ViewProjection viewProjection_);

	/// <summary>
	/// マウスの設定
	/// </summary>
	void SettingMouse(const ViewProjection viewProjection_);

	/// <summary>
	/// HPの設定
	/// </summary>
	void SetHp();

	//HPの描画
	void DrawHP();

};