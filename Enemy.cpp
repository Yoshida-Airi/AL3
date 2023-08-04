#include "Enemy.h"

Enemy::~Enemy()
{ delete state; }

void Enemy::Initialize(
    Model* model, const Vector3& position) {
	// NULLポインタチェック
	assert(model);

	model_ = model;

	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("black.png");
	// ワールド変換の初期化
	worldTransform_.Initialize();
	// 初期座標の設定
	worldTransform_.translation_ = position;

	state = new EnemyStateApproach();
	state->Initialize(this);


}

void Enemy::Update() 
{


	// 行列更新
	worldTransform_.UpdateMatrix();

	state->Update(this);

}

// 描画
void Enemy::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}



// 移動
void Enemy::Move(Vector3& velocity) { SumVector3(worldTransform_.translation_, velocity); }

// フェーズの以降
void Enemy::ChangeState(BaseEnemyState* newState) { state = newState; }