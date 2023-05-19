#include "Enemy.h"

void Enemy::Initialize(Model* model, const Vector3& position,const Vector3& velocity)
{
	// NULLポインタチェック
	assert(model);

	model_ = model;

	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("black.png");
	// ワールド変換の初期化
	worldTransform_.Initialize();
	//初期座標の設定
	worldTransform_.translation_ = position;
	//引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;

}

void Enemy::Update()
{ 
	

	// 座標を移動させる(1フレーム分の移動量を足しこむ)
	AffinMatrix_.SumVector3(worldTransform_.translation_, velocity_);
	//行列更新
	worldTransform_.UpdateMatrix(); 
}

void Enemy::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_); 
}