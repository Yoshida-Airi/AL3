#pragma once
#include<Model.h>
#include<WorldTransform.h>
#include"MathUtility.h"

/// <summary>
/// 敵の弾
/// </summary>
class EnemyBullet
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

private:

	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// 速度
	Vector3 velocity_;

};
