#include "Enemy.h"

Enemy::Enemy() 
{ 
	state = new EnemyStateApproach();
	ApprochVelocity_ = {0, 0, -0.5f};
	LeaveVelocity_ = {-0.5f, 0.5f, 0};
}

Enemy::~Enemy()
{
	//解放処理
	delete state; 

	for (EnemyBullet* bullet : this->bullets_)
	{
		delete bullet;
	}

	for (TimedCall* timeCall : this->timedCalls_) 
	{
		delete timeCall;
	}

}

void Enemy::Initialize(Model* model, const Vector3& position)
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
	isAttacEvent = true;
	// 接近フェーズ初期化
	ApproachInitialize();
	// 発射タイマーの初期化
	AttackReset();
	
}



void Enemy::Update() 
{
	// 行列更新
	worldTransform_.UpdateMatrix();

	//接近フェーズの呼び出し
	Approach();

	if (worldTransform_.translation_.z < 0.0f)
	{
		isAttacEvent = false;
		Leave();
	}

	// 終了したタイマーを削除
	timedCalls_.remove_if([](TimedCall* timeCall) {
		if (timeCall->isFinished() == true) {
			delete timeCall;
			return true;
		}
		return false;
	});

		// 範囲forでリストの全要素について回す
	for (TimedCall* timedCall : this->timedCalls_)
	{
		timedCall->Update();
	}
	

	//弾更新
	for (EnemyBullet* bullet : this->bullets_)
	{
		bullet->Update();
	}


}

// 弾を発射した後にリセット
void Enemy::AttackReset()
{
	//弾を発射
	if (isAttacEvent == true)
	{
		Fire();
	}
	// 発射タイマーをセットする
	timedCalls_.push_back(new TimedCall(std::bind(&Enemy::AttackReset, this), kFireInterval));
}

// 移動
void Enemy::Move(Vector3& velocity)
{
	AffinMatrix_.SumVector3(worldTransform_.translation_, velocity);
}

// 弾の発射
void Enemy::Fire()
{
	// 弾の速度
	const float kBulletSpeed = -1.0f;
	Vector3 velocity(0, 0, kBulletSpeed);

	// 弾を生成し、初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	// 弾を登録する
	bullets_.push_back(newBullet);
}

// 描画
void Enemy::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	//弾描画
	for (EnemyBullet* bullet : this->bullets_)
	{
		bullet->Draw(viewProjection);
	}
}


// 接近フェーズ初期化
void Enemy::ApproachInitialize() 
{
	
}


// 接近フェーズ(更新)
void Enemy::Approach()
{ 
	//発射タイマーカウントダウン
	timer--;
	if (timer == 0)
	{
		//発射タイマーを初期化
		timer = kFireInterval;
	}

	state->update(this, ApprochVelocity_); 
}

// 離脱フェーズ
void Enemy::Leave() 
{
	state->update(this, LeaveVelocity_); 
}

//フェーズの以降
void Enemy::ChangeState(BaseEnemyState* newState)
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

