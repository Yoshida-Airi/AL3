#include "EnemyBullet.h"


// デストラクタ
EnemyBullet::~EnemyBullet() {}

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	// NULLポインタチェック
	assert(model);

	model_ = model;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("Red.png");

	// ワールド変換の初期化
	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
	// 引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;

	// Z方向に伸びた形状
	worldTransform_.scale_.x = 0.5f;
	worldTransform_.scale_.y = 0.5f;
	worldTransform_.scale_.z = 3.0f;


}

void EnemyBullet::Update() {
	
	//***********************************************//
	//					　ホーミング					 //
	//***********************************************//

	//敵弾から自キャラへのベクトルを計算
	Vector3 toPlayer = Subtract(player_->GetWorldPosition(), this->worldTransform_.translation_);

	//ベクトルを正規化する
	toPlayer = Normalize(toPlayer);
	velocity_ = Normalize(velocity_);

	//球面線形補完により、今の速度と自キャラへのベクトルを内挿し、新たな速度とする
	velocity_ = Slerp(velocity_, toPlayer, 0.1f);
	velocity_.x *= 2.0f;
	velocity_.y *= 2.0f;
	velocity_.z *= 2.0f;

	//***********************************************//
	//			　　　弾を進行方向に向ける				 //
	//***********************************************//
	// Y軸周り角度(θy)
	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);
	// 横軸方向の長さを求める
	float velocityXZ = sqrt(velocity_.x * velocity_.x + velocity_.z * velocity_.z);
	// X軸周りの角度(θx)
	worldTransform_.rotation_.x = std::atan2(-velocity_.y, velocityXZ);



	// 座標を移動させる(1フレーム分の移動量を足しこむ)
	SumVector3(worldTransform_.translation_, velocity_);
	// 行列更新
	worldTransform_.UpdateMatrix();

	//時間経過でデス
	if (--deathTimer_ <= 0)
	{
		isDead_ = true;
	}


}

void EnemyBullet::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void EnemyBullet::OnCollision()
{ 
	//デスフラグを立てる
	isDead_ = true; 
}
