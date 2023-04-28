#include"Player.h"
#include<cassert>
void Player::Initialize(Model* model, uint32_t texturehandle)
{
	//NULLポインタチェック
	assert(model);

	//引数として受け取ったデータをメンバ変数に記録する
	textureHandle_ = texturehandle;

	//ワールド変換の初期化
	worldTransform_.Initialize();

};

void Player::Update()
{
	//行列を定数バッファに転送
	worldTransform_.TransferMatrix();
};

void Player::Draw(ViewProjection viewProjection_)
{
	
	//3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
}