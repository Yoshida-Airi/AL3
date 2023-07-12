#include "Enemy.h"

Enemy::Enemy() 
{ 
	state = new EnemyStateApproach();
}

Enemy::~Enemy()
{
	delete state; 
}

void Enemy::Initialize( Model* model, const Vector3& position)
{
	// NULLポインタチェック
	assert(model);

	model_ = model;

	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("black.png");
	// ワールド変換の初期化
	worldTransform_.Initialize();
	// 初期座標の設定
	worldTransform_.translation_ = position;



}

void Enemy::Update() 
{
	state->update(this);
	// 行列更新
	worldTransform_.UpdateMatrix();
}

// 描画
void Enemy::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}


void Enemy::ChangeState(BaseEnemyState* newState)
{
	delete state; 
	state = newState;
}

//移動
void Enemy::Move(Vector3& velocity) {
	AffinMatrix_.SumVector3(worldTransform_.translation_, velocity);
}


