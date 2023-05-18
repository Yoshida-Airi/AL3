#include <PlayerBullet.h>
#include<cassert>
void PlayerBullet::Initialize(Model* model, const Vector3& position)
{
	//NULLポインタチェック
	assert(model);

	model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("black.png");

	
	//引数で受け取った初期座標をセット
	worldTransform_.translation_;
}

void PlayerBullet::Update()
{

}

void PlayerBullet::Draw(ViewProjection viewProjection)
{

}