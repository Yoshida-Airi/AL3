#include "Ground.h"

void Ground::Initialize(Model* model, const Vector3& position) {
	// NULLポインタチェック
	assert(model);
	model_ = model;

	// ワールド変換の初期化
	worldTransform_.Initialize();

	worldTransform_.translation_ = position;

	// 天球の大きさ(大きさによってはカメラのfarZをSetterで設定しよう)

	worldTransform_.scale_.x = 100.0f;
	worldTransform_.scale_.y = 100.0f;
	worldTransform_.scale_.z = 100.0f;
}

void Ground::Update()
{
	// 行列の更新
	worldTransform_.UpdateMatrix();
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
}

void Ground::Draw(ViewProjection viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_);
}
