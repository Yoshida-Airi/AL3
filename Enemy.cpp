#include "Enemy.h"

Enemy::Enemy() 
{ 
	state = new EnemyStateApproach();
	ApprochVelocity_ = {0, 0, -0.5f};
	LeaveVelocity_ = {-0.5f, 0.5f, 0};
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
	
	// 行列更新
	worldTransform_.UpdateMatrix();

	Approach();
	if (worldTransform_.translation_.z < 0.0f)
	{
		Leave();
	}


}

// 描画
void Enemy::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

// 接近フェーズ
void Enemy::Approach()
{ 
	state->update(this, ApprochVelocity_); 
}


// 離脱フェーズ
void Enemy::Leave() 
{ 
	state->update(this, LeaveVelocity_);
}


void (Enemy::*Enemy::spFuncTable[])() =
{
	&Enemy::Approach,	//接近
	&Enemy::Leave		//離脱
};


void Enemy::ChangeState(BaseEnemyState*newState)
{
	delete state; 
	state = newState;
}


void EnemyStateApproach::update(Enemy* pEnemy, Vector3& velocity)
{ 
	pEnemy->Move(velocity);
	pEnemy->ChangeState(new EnemyStateLeave());
}

void EnemyStateLeave::update(Enemy* pEnemy, Vector3& velocity)
{ 
	pEnemy->Move(velocity);
}

//移動
void Enemy::Move(Vector3& velocity) 
{
	AffinMatrix_.SumVector3(worldTransform_.translation_, velocity);
}
