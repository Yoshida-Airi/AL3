#include "Enemy.h"
#include "player.h"
#include"GameScene.h"


Enemy::Enemy() {

}

Enemy::~Enemy() {
	// 解放処理
	delete state;


	for (TimedCall* timeCall : this->timedCalls_) {
		delete timeCall;
	}
}

void Enemy::Initialize(Model* model, const Vector3& position) {
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

void Enemy::Update() {
	// 行列更新
	worldTransform_.UpdateMatrix();

	//敵の状態遷移
	state->update(this);

	// 終了したタイマーを削除
	timedCalls_.remove_if([](TimedCall* timeCall) {
		if (timeCall->isFinished() == true) {
			delete timeCall;
			return true;
		}
		return false;
	});

	// 範囲forでリストの全要素について回す
	for (TimedCall* timedCall : this->timedCalls_) {
		timedCall->Update();
	}

	worldTransform_.TransferMatrix();

}

// 弾を発射した後にリセット
void Enemy::AttackReset() {
	// 弾を発射

	Fire();

	// 発射タイマーをセットする
	timedCalls_.push_back(new TimedCall(std::bind(&Enemy::AttackReset, this), kFireInterval));
}

// 移動
void Enemy::Move(Vector3& velocity) { SumVector3(worldTransform_.translation_, velocity); }

// 弾の発射
void Enemy::Fire() {
	// 弾の速度
	const float kBulletSpeed = 1.0f;
	/*Vector3 velocity(0, 0, kBulletSpeed);*/

	// 自キャラのワールド座標を取得する
	Vector3 worldPlayer = player_->GetWorldPosition();
	// 敵キャラのワールド座標を取得する
	Vector3 worldEnemy = GetWorldPosition();
	// 敵キャラ→自キャラの差分ベクトルを求める
	Vector3 velocity = Subtract(worldPlayer, worldEnemy);
	// 正規化
	velocity = Normalize(velocity);

	// ベクトルの長さを速さに合わせる
	velocity.x *= kBulletSpeed;
	velocity.y *= kBulletSpeed;
	velocity.z *= kBulletSpeed;


	// 弾を生成し、初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_,worldTransform_.translation_, velocity);
	newBullet->SetPlayer(player_);
	// 弾を登録する
	gameScene_->AddEnemyBullet(newBullet);

}

// 描画
void Enemy::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}



// フェーズの以降
void Enemy::ChangeState(BaseEnemyState* newState) {
	state = newState;
}

//当たり判定
void Enemy::OnCollision() { isDead_ = true; };

void EnemyStateApproach::Initialize(Enemy* pEnemy) { pEnemy->AttackReset(); }

void EnemyStateApproach::update(Enemy* pEnemy) {
	// 速度
	Vector3 velocity;
	velocity = {0, 0, -0.0f};
	pEnemy->Move(velocity);
	if (pEnemy->GetWorldPosition().z < 0.0f) {
		pEnemy->ChangeState(new EnemyStateLeave());
	}
}

void EnemyStateLeave::Initialize(Enemy* pEnemy) { pEnemy->GetWorldPosition(); }

void EnemyStateLeave::update(Enemy* pEnemy) {

	Vector3 velocity;
	velocity = {-0.5f, 0.5f, 0};
	pEnemy->Move(velocity);
}

// セッター
void Enemy::SetPlayer(Player* player)
{
	player_ = player;
}

void Enemy::SetGameScene(GameScene* gameScene) 
{ 
	gameScene_ = gameScene; 
}