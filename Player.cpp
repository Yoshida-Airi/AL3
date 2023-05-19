#include "Player.h"
#include "ImGuiManager.h"
#include "Vector.h"
#include "WorldAffinMatrix.h"
#include <cassert>

Player::~Player()
{
	for (PlayerBullet* bullet : bullets_)
	{
		delete bullet;
	}
}

void Player::Initialize(Model* model, uint32_t textureHandle) 
{
	// NULLポインタチェック
	assert(model);

	// 引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	textureHandle_ = textureHandle;

	// ワールド変換の初期化
	worldTransform_.Initialize();

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};
}

void Player::Rotate()
{	
	const float kRotSpeed = 0.02f;

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_A))
	{
		worldTransform_.rotation_.y -= kRotSpeed;
	}
	else if (input_->PushKey(DIK_D))
	{
		worldTransform_.rotation_.y += kRotSpeed;	
	}
}

// 攻撃処理
void Player::Attack()
{
	if (input_->TriggerKey(DIK_SPACE))
	{
		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_,worldTransform_.translation_);

		// 弾を登録する
		bullets_.push_back(newBullet);
	}
}

void Player::Update() 
{

	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};

	// キャラクター旋回処理
	Rotate();

	// キャラクターの移動速さ
	const float kCharcterSpeed = 0.2f;

	// 押した方向で移動ベクトルを変更（左右）
	if (input_->PushKey(DIK_LEFT))
	{
		move.x -= kCharcterSpeed;
	}
	else if (input_->PushKey(DIK_RIGHT))
	{
		move.x += kCharcterSpeed;
	}

	// 押した方向で移動ベクトルを変更（上下）
	if (input_->PushKey(DIK_UP))
	{
		move.y += kCharcterSpeed;
	} 
	else if (input_->PushKey(DIK_DOWN)) 
	{
		move.y -= kCharcterSpeed;
	}


	// 座標移動(ベクトルの加算)
	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;

	// キャラクター攻撃
	Attack();

	// 弾更新
	for (PlayerBullet* bullet : bullets_)
	{
		bullet->Update();
	}


	// 移動限界座標
	const float kMoveLimitX = 33.0f;
	const float kMoveLimitY = 18.0f;

	// 範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	//行列更新
	worldTransform_.UpdateMatrix();

	// キャラクターの座標を画面表示する処理
	ImGui::Begin("Debug1");
	float playerPos[] =
	{
	    worldTransform_.translation_.x, 
		worldTransform_.translation_.y,
	    worldTransform_.translation_.z
	};
	// float3スライダー
	ImGui::SliderFloat3("player", playerPos, 100.0f, -100.0f);

	ImGui::Text("DebugCamera : C");
	ImGui::Text("Move : CursorKey");

	worldTransform_.translation_.x = playerPos[0];
	worldTransform_.translation_.y = playerPos[1];
	worldTransform_.translation_.z = playerPos[2];

	ImGui::End();
}

void Player::Draw(ViewProjection viewProjection_)
{
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);

	for (PlayerBullet* bullet : bullets_)
	{
		bullet->Draw(viewProjection_);
	}
}

