#include "Player.h"
#include<cassert>
#include"MathFunction.h"
#include"Vector.h"
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
}

void Player::Update()
{
	//行列を定数バッファに転送
	worldTransform_.TransferMatrix();

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

	worldTransform_.matWorld_ = MakeAffinMatrix
	(
	    worldTransform_.scale_,
		worldTransform_.rotation_, 
		worldTransform_.translation_
	);

	//行列を定数バッファに転送
	worldTransform_.TransferMatrix();


}

void Player::Draw(ViewProjection viewProjection_) 
{
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);

}