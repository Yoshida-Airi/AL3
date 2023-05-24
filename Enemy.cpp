#include "Enemy.h"

void Enemy::Initialize(
    Model* model, const Vector3& position, const Vector3& velocityA, const Vector3& velocityB) {
	// NULLポインタチェック
	assert(model);

	model_ = model;

	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("black.png");
	// ワールド変換の初期化
	worldTransform_.Initialize();
	// 初期座標の設定
	worldTransform_.translation_ = position;
	// 引数で受け取った速度をメンバ変数に代入
	ApprochVelocity_ = velocityA;
	LeaveVelocity_ = velocityB;
}

void Enemy::Update() 
{
	
	(this->*spFuncTable[static_cast<size_t>(phase_)])();

	// 行列更新
	worldTransform_.UpdateMatrix();


}

// 描画
void Enemy::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

// 接近フェーズ
void Enemy::Approach()
{
	// 移動
	AffinMatrix_.SumVector3(worldTransform_.translation_, ApprochVelocity_);
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}

// 離脱フェーズ
void Enemy::Leave()
{
	// 移動(ベクトルを加算)
	AffinMatrix_.SumVector3(worldTransform_.translation_, LeaveVelocity_);
}


void (Enemy::*Enemy::spFuncTable[])() =
{
	&Enemy::Approach,	//接近
	&Enemy::Leave		//離脱
};