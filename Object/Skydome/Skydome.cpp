#include "Skydome.h"

/// <summary>
/// 初期化
/// </summary>
void Skydome::Initialize(Model* model, const Vector3& position) {
	
	// NULLポインタチェック
	assert(model);
	model_ = model;

	//ワールド変換の初期化
	worldTransform_.Initialize();

	worldTransform_.translation_ = position;

	// 天球の大きさ(大きさによってはカメラのfarZをSetterで設定しよう)
	

	worldTransform_.scale_.x = 500.0f;
	worldTransform_.scale_.y = 500.0f;
	worldTransform_.scale_.z = 500.0f;

}


/// <summary>
/// 更新
/// </summary>
void Skydome::Update()
{
	//行列の更新
	worldTransform_.UpdateMatrix();
	//行列を定数バッファに転送
	worldTransform_.TransferMatrix();
}

/// <summary>
/// 描画
/// </summary>
void Skydome::Draw(ViewProjection viewProjection_) {

	model_->Draw(worldTransform_, viewProjection_);

}