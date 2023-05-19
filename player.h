#pragma once
#include "Model.h"
#include "WorldAffinMatrix.h"
#include"Input.h"
#include"DebugCamera.h"
#include"PlayerBullet.h"

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

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	WorldTransformEX transform;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// キーボード入力
	Input* input_ = nullptr;
	// ImGuiで値を入力する変数
	float inputFloat3[3] = {0, 0, 0};
	//弾
	PlayerBullet* bullet_ = nullptr;

	//攻撃
	void Attack();

};