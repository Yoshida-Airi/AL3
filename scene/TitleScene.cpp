#include "TitleScene.h"


void TitleScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	scene_ = TITLE;
	input_ = Input::GetInstance();
	titleTexture = TextureManager::Load("Title.png");
	
	audio_ = Audio::GetInstance();
	soundData_ = audio_->LoadWave("gameScene.wav");

	
	audio_->PlayWave(soundData_, true);
	
}

void TitleScene::Update()
{
	ifAudio = true;

	title_ = Sprite::Create(titleTexture, {640.0f, 360.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});

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

	if (sceneState_ == true)
	{
		scene_ = GAME;
	}

}

void TitleScene::Draw() 
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();


	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	title_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();
}

void TitleScene::Reset()
{ 
	sceneState_ = false;
}