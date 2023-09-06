#include "Player.h"
#include "ImGuiManager.h"
#include "Vector.h"
#include <cassert>

Player::~Player() {

	delete sprite2DReticle_;
}

void Player::Initialize(Model* model, uint32_t textureHandle,Vector3 positon) {
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
	worldTransform_.translation_ = positon;



	
}



// 攻撃処理
void Player::Attack() {

	XINPUT_STATE joyState;

	//ゲームパット未接続ならキーボードで操作
	if (!Input::GetInstance()->GetJoystickState(0, joyState))
	{
		if (input_->TriggerKey(DIK_SPACE)) {
			// 弾の速度
			const float kBulletSpeed = 1.0f;
			Vector3 velocity;

			// 自機から照準オブジェクトへのベクトル
			velocity = Subtract(GetReticleWorldPosition(), GetWorldPosition());
			velocity = Multiply(kBulletSpeed, Normalize(velocity));

			// 速度ベクトルを自機の向きに合わせて回転させる
			velocity = TransformNormal(velocity, worldTransform_.matWorld_);

			// 弾を生成し、初期化
			PlayerBullet* newBullet = new PlayerBullet();
			newBullet->Initialize(model_, GetWorldPosition(), velocity);

		}
	}

	if (joyState.Gamepad.wButtons&XINPUT_GAMEPAD_RIGHT_SHOULDER) {
		// 弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity;

		// 速度ベクトルを自機の向きに合わせて回転させる
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		//自機から照準オブジェクトへのベクトル
		velocity = Subtract(GetReticleWorldPosition(), GetWorldPosition());
		velocity = Multiply(kBulletSpeed, Normalize(velocity));


	}
}

void Player::Update(const ViewProjection viewProjection_) {

	// ゲームパットの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;


	Vector3 move = {0, 0, 0};
	// しきい値
	const float threshold = 0.7f;
	bool isMoving = false;


	// キャラクターの移動速さ
	const float kCharcterSpeed = 0.3f;



	if (Input::GetInstance()->GetJoystickState(0, joyState))
	{
		// キャラクターの移動ベクトル
		move = {
		    (float)joyState.Gamepad.sThumbLX / SHRT_MAX, 0.0f,
		    (float)joyState.Gamepad.sThumbLY / SHRT_MAX
		};

		if (Length(move) > threshold)
		{
			isMoving = true;
		}

		if (isMoving) {
			// 移動量に速さを反映
			Normalize(move);
			move = Multiply(kCharcterSpeed, move);

		}

	}

	// 押した方向で移動ベクトルを変更（左右）
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharcterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharcterSpeed;
	}

	// 押した方向で移動ベクトルを変更（上下）
	if (input_->PushKey(DIK_UP)) {
		move.z += kCharcterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.z -= kCharcterSpeed;
	}

	
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);


	// Y軸周り角度(θy)
	worldTransform_.rotation_.y = std::atan2(move.x, move.z);
	


	// 行列更新
	worldTransform_.UpdateMatrix();

	SettingMouse(viewProjection_);

	//SettingReticle(viewProjection_);

	worldTransform_.TransferMatrix();

}

void Player::Draw(ViewProjection viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);


}

void Player::DrawUI() 
{ 
	
}

// ワールド座標を取得
Vector3 Player::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldpos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldpos.x = worldTransform_.matWorld_.m[3][0];
	worldpos.y = worldTransform_.matWorld_.m[3][1];
	worldpos.z = worldTransform_.matWorld_.m[3][2];

	return worldpos;
}

Vector3 Player::GetReticleWorldPosition()
{
	// ワールド座標を入れる変数
	Vector3 worldpos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldpos.x = worldTransform3DReticle_.matWorld_.m[3][0];
	worldpos.y = worldTransform3DReticle_.matWorld_.m[3][1];
	worldpos.z = worldTransform3DReticle_.matWorld_.m[3][2];

	return worldpos;
}

void Player::OnCollision()
{

}

//親子関係を結ぶ
void Player::SetParent(const WorldTransform* parent)
{
	worldTransform_.parent_ = parent; 
	worldTransform3DReticle_.parent_ = parent;
}


void Player::SettingReticle(const ViewProjection viewProjection_)
{

	//// 自機のワールド座標から3Dレティクルのワールド座標を計算
	//// 自機から3Dレティクルへの距離
	//const float kDistancePlayerTo3DReticle = 50.0f;
	//// 自機から3Dレティクルへのオフセット(Z+向き)
	//Vector3 offset = {0, 0, 1.0f};
	//// 自機のワールド行列の回転を反映
	//offset = TransformNormal(offset, worldTransform_.matWorld_);
	//// ベクトルの長さを整える
	//offset = Multiply(kDistancePlayerTo3DReticle, Normalize(offset));
	//// 3Dレティクルの座標を設定
	//worldTransform3DReticle_.translation_ = Add(GetWorldPosition(), offset);
	//// ワールド行列の更新
	//worldTransform3DReticle_.UpdateMatrix();
	//worldTransform3DReticle_.TransferMatrix();

	//// 3Dレティクルのワールド座標から2Dレティクルのスクリーン座標を計算
	//Vector3 positionReticle = GetReticleWorldPosition();
	//// ビューポート行列
	//Matrix4x4 matViewport =
	//    MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	//// ビュー行列とプロジェクション行列、ビューポート行列を合成する
	//Matrix4x4 matViewProjectionViewport =
	//    Multiply(viewProjection_.matView, Multiply(viewProjection_.matProjection, matViewport));
	//// ワールド→スクリーン座標変換(ここまで3Dから2Dになる)
	//positionReticle = Transform(positionReticle, matViewProjectionViewport);
	//// スプライトのレティクルに座標設定
	//sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));

	//ImGui::Begin("Player");
	//ImGui::Text(
	//    "2DReticle:(%f,%f)", sprite2DReticle_->GetPosition().x, sprite2DReticle_->GetPosition().y);

	//ImGui::Text(
	//    "3DReticle:(%+.2f,%+.2f,%+.2f)", worldTransform3DReticle_.translation_.x,
	//    worldTransform3DReticle_.translation_.y, worldTransform3DReticle_.translation_.z);
	//ImGui::End();


}


void Player::SettingMouse(const ViewProjection viewProjection_)
{ 
	
	//// 自機のワールド座標から3Dレティクルのワールド座標を計算
	//// 自機から3Dレティクルへの距離
	//const float kDistancePlayerTo3DReticle = 50.0f;
	//// 自機から3Dレティクルへのオフセット(Z+向き)
	//Vector3 offset = {0, 0, 1.0f};
	//// 自機のワールド行列の回転を反映
	//offset = TransformNormal(offset, worldTransform_.matWorld_);
	//// ベクトルの長さを整える
	//offset = Multiply(kDistancePlayerTo3DReticle, Normalize(offset));
	//// 3Dレティクルの座標を設定
	//worldTransform3DReticle_.translation_ = Add(GetWorldPosition(), offset);
	//// ワールド行列の更新
	//worldTransform3DReticle_.UpdateMatrix();
	//worldTransform3DReticle_.TransferMatrix();

	//// 3Dレティクルのワールド座標から2Dレティクルのスクリーン座標を計算
	//Vector3 positionReticle = GetReticleWorldPosition();
	//// ビューポート行列
	//Matrix4x4 matViewport =
	//    MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	//// ビュー行列とプロジェクション行列、ビューポート行列を合成する
	//Matrix4x4 matViewProjectionViewport =
	//    Multiply(viewProjection_.matView, Multiply(viewProjection_.matProjection, matViewport));
	//// ワールド→スクリーン座標変換(ここまで3Dから2Dになる)
	//positionReticle = Transform(positionReticle, matViewProjectionViewport);
	//// スプライトのレティクルに座標設定
	//sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));

	//POINT mousePosition;
	////マウス座標(スクリーン座標)を取得する
	//GetCursorPos(&mousePosition);

	////クライアントエリア座標に変換する
	//HWND hwnd = WinApp::GetInstance()->GetHwnd();
	//ScreenToClient(hwnd, &mousePosition);

	////マウス座標を2Dレティクルのスプライトに代入する
	//sprite2DReticle_->SetPosition(Vector2(float(mousePosition.x), float(mousePosition.y)));
//
//	// ビュー行列とプロジェクション行列、ビューポート行列を合成する
//	Matrix4x4 matVPV =
//	    Multiply(viewProjection_.matView, Multiply(viewProjection_.matProjection, matViewport));
//
//	//合成行列の逆行列を計算する
//	Matrix4x4 matInverseVPV = Inverse(matVPV);
//
//	//スクリーン座標
//	Vector3 posNear =
//	    Vector3(sprite2DReticle_->GetPosition().x, sprite2DReticle_->GetPosition().y, 0);
//	Vector3 posFar =
//	    Vector3(sprite2DReticle_->GetPosition().x, sprite2DReticle_->GetPosition().y, 1);
//
//	Vector2 spritePosirion = sprite2DReticle_->GetPosition();
//
//	XINPUT_STATE joyState;
//
//	if (Input::GetInstance()->GetJoystickState(0, joyState))
//	{
//		spritePosirion.x += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * 5.0f;
//		spritePosirion.y -= (float)joyState.Gamepad.sThumbRY / SHRT_MAX * 5.0f;
//
//		sprite2DReticle_->SetPosition(spritePosirion);
//
//	}
//
//
//	//スクリーン座標系からワールド座標系へ
//	posNear = Transform(posNear, matInverseVPV);
//	posFar = Transform(posFar, matInverseVPV);
//
//	//マウスレイの方向
//	Vector3 mouseDirection = Subtract(posFar, posNear);
//	mouseDirection = Normalize(mouseDirection);
//
//	//カメラから照準オブジェクトの距離
//	const float kDistanceTestObject = 50.0f;
//	worldTransform3DReticle_.translation_ =
//	    Add(offset, Add(posNear, Multiply(kDistanceTestObject, mouseDirection)));
//
//	worldTransform3DReticle_.UpdateMatrix();
//
//	ImGui::Begin("Player");
//	ImGui::Text(
//	    "2DReticle:(%f,%f)", sprite2DReticle_->GetPosition().x, sprite2DReticle_->GetPosition().y);
//	ImGui::Text("Near:(%+2f,%+2f,%+2f)", posNear.x, posNear.y, posNear.z);
//	ImGui::Text("Far:(%+2f,%+2f,%+2f)", posFar.x, posFar.y, posFar.z);
//	ImGui::Text(
//	    "3DReticle:(%+2f,%+2f,%+2f)", worldTransform3DReticle_.translation_.x,
//	    worldTransform3DReticle_.translation_.y, worldTransform3DReticle_.translation_.z);
//	ImGui::End();
//
//
}
