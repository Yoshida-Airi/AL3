#include "Rain.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="model">モデル</param>
/// <param name="textureHandle">テクスチャハンドル</param>
void Rain::Initialize(Model* model, const Vector3& position) {
	// NULLポインタチェック
	assert(model);
	model_ = model;

	// ワールド変換の初期化
	worldTransform_.Initialize();

	worldTransform_.translation_ = position;

	
	// 雨の大きさ
	size_ = 0.2f;
	worldTransform_.scale_ = {size_, size_, size_};

	//速度
	velocity_ = {0, 0};

	rainDensity_ = 100.0f;         // 雨粒の密度
	airDensity_ = 1.293f;           // 空気の密度
	k_ = 0.5f ;               // 空気抵抗係数
	kGravity_ = {0.0f, -9.8f , 0.0f}; // 重力加速度

	//風速
	windSpeed_ = {00.0f, 0.0f, 0.0f};

}

/// <summary>
/// 更新
/// </summary>
void Rain::Update()
{
	if (isActive_ == false)
	{
		// エミッターの位置からランダムに位置を決める
		Vector3 min;
		Vector3 max;

		min.x = -40.0f;
		max.x = 40.0f;
		min.y = 20.0f;
		max.y = 80.0f;
		min.z = -40.0f;
		max.z = 20.0f;

		this->worldTransform_.translation_.x =
		    min.x + static_cast<float>(rand()) / RAND_MAX * (max.x - min.x);
		this->worldTransform_.translation_.y =
		    min.y + static_cast<float>(rand()) / RAND_MAX * (max.y - min.y);
		this->worldTransform_.translation_.z =
		    min.z + static_cast<float>(rand()) / RAND_MAX * (max.z - min.z);

		isActive_ = true;
	}
	
	if (isActive_ == true) {

		Vector3 wind = {0, 0, 0};
		

		// 風力を求める


		wind.x = 0.5f * airDensity_ * static_cast<float>(pow((windSpeed_.x), 2.0f));
		wind.y = 0.5f * airDensity_ * static_cast<float>(pow((windSpeed_.y), 2.0f));
		wind.z = 0.5f * airDensity_ * static_cast<float>(pow((windSpeed_.z), 2.0f));
		       
	

		// 終端速度を求める
		Vector3 terminalVelocity;
		terminalVelocity.x =
		    (8.0f / 3.0f) * ((size_ / 2.0f) * kGravity_.x * rainDensity_) / (k_ * airDensity_);
		terminalVelocity.y =
		    (8.0f / 3.0f) * ((size_ / 2.0f) * kGravity_.y * rainDensity_) / (k_ * airDensity_);
		terminalVelocity.z =
		    (8.0f / 3.0f) * ((size_ / 2.0f) * kGravity_.z * rainDensity_) / (k_ * airDensity_);

		Vector3 acceleration;
		acceleration = {0.0f, kGravity_.y, 0.0f}; // 加速度

		// 速度の更新
		this->velocity_.x += (terminalVelocity.x + wind.x) * (1.0f / 60.0f);
		this->velocity_.y += (terminalVelocity.y + wind.y) * (1.0f / 60.0f);
		this->velocity_.z += (terminalVelocity.z + wind.z) * (1.0f / 60.0f);

		worldTransform_.translation_.x += velocity_.x * (1.0f / 60.0f);
		worldTransform_.translation_.y += velocity_.y * (1.0f / 60.0f);
		worldTransform_.translation_.z += velocity_.z * (1.0f / 60.0f);

		
		if (worldTransform_.translation_.y <= -20 || worldTransform_.translation_.y >= 80)
		{
			isActive_ = false;
			velocity_ = {0, 0, 0};
			terminalVelocity = {0, 0, 0};
			worldTransform_.translation_ = {0, 30, 0};
			wind = {0, 0, 0};
		}

		
	}

	// 行列更新
	worldTransform_.UpdateMatrix();

	ImGui::Begin("particle");

	float wind[] = {windSpeed_.x, windSpeed_.y, windSpeed_.z};

	ImGui::SliderFloat3("windSpeed", wind, 0, 50);

	windSpeed_.x = wind[0];
	windSpeed_.y = wind[1];
	windSpeed_.z = wind[2];




	ImGui::End();
}

/// <summary>
/// 描画
/// </summary>
/// <param name="viewProjection"ビュープロジェクション（参照渡し）</param>
void Rain::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}


