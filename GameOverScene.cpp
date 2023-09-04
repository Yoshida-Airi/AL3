#include "GameOverScene.h"


void GameOverScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	scene_ = OVER;
	input_ = Input::GetInstance();
	Texture = TextureManager::Load("Over.png");


}

void GameOverScene::Update() {
	GameOver_ = Sprite::Create(Texture, {640.0f, 360.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});



	// ゲームパットの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
			
			sceneState_ = true;
		}
	}

	if (input_->PushKey(DIK_SPACE)) {
		
		sceneState_ = true;
	}


}

void GameOverScene::Draw() {
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	GameOver_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();
}

void GameOverScene::Reset()
{ sceneState_ = false; }