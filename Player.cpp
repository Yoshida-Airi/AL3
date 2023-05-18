#include "Player.h"
#include<cassert>
#include "WorldTransform.h"
#include"Vector.h"
#include"ImGuiManager.h"
void Player::Initialize(Model* model, uint32_t textureHandle)
{
	//NULLポインタチェック
	assert(model);

	//引数として受け取ったデータをメンバ変数に記録する
	model_ = model;  
	textureHandle_ = textureHandle;
	
	//ワールド変換の初期化
	worldTransform_.Initialize();

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

	
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};


}

void Player::Update()
{
	
	//キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};

	//キャラクターの移動速さ
	const float kCharcterSpeed = 0.2f;

	//押した方向で移動ベクトルを変更（左右）
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

	//座標移動(ベクトルの加算)
	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;

	//移動限界座標
	const float kMoveLimitX = 33.0f;
	const float kMoveLimitY = 18.0f;

	//範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);


	//行列更新
	worldTransform_.matWorld_ = MakeAffinMatrix
	(
	    worldTransform_.scale_,
		worldTransform_.rotation_, 
		worldTransform_.translation_
	);



	//行列を定数バッファに転送
	worldTransform_.TransferMatrix();


	//キャラクターの座標を画面表示する処理
	ImGui::Begin("Debug1");
	float playerPos[] = 
{
		worldTransform_.translation_.x ,
		worldTransform_.translation_.y,
	    worldTransform_.translation_.z
	};
	// float3スライダー
	ImGui::SliderFloat3
	(
	    "player",
		playerPos, 
		100.0f,
		-100.0f
	);
	
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

}