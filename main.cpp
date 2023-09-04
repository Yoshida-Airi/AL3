#include "Audio.h"
#include "AxisIndicator.h"
#include "DirectXCommon.h"
#include "GameScene.h"
#include "ImGuiManager.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include "WinApp.h"
#include"SceneState.h"
#include"TitleScene.h"
#include"GameOverScene.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	WinApp* win = nullptr;
	DirectXCommon* dxCommon = nullptr;
	// 汎用機能
	Input* input = nullptr;
	Audio* audio = nullptr;
	AxisIndicator* axisIndicator = nullptr;
	PrimitiveDrawer* primitiveDrawer = nullptr;

	TitleScene* titleScene = nullptr;
	GameScene* gameScene = nullptr;
	GameOverScene* gameoverScene = nullptr;

	// ゲームウィンドウの作成
	win = WinApp::GetInstance();
	win->CreateGameWindow(L"LE2B_28_ヨシダ_アイリ_タコの惑星");

	// DirectX初期化処理
	dxCommon = DirectXCommon::GetInstance();
	dxCommon->Initialize(win);

#pragma region 汎用機能初期化
	// ImGuiの初期化
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();
	imguiManager->Initialize(win, dxCommon);

	// 入力の初期化
	input = Input::GetInstance();
	input->Initialize();

	// オーディオの初期化
	audio = Audio::GetInstance();
	audio->Initialize();

	// テクスチャマネージャの初期化
	TextureManager::GetInstance()->Initialize(dxCommon->GetDevice());
	TextureManager::Load("white1x1.png");

	// スプライト静的初期化
	Sprite::StaticInitialize(dxCommon->GetDevice(), WinApp::kWindowWidth, WinApp::kWindowHeight);

	// 3Dモデル静的初期化
	Model::StaticInitialize();

	// 軸方向表示初期化
	axisIndicator = AxisIndicator::GetInstance();
	axisIndicator->Initialize();

	primitiveDrawer = PrimitiveDrawer::GetInstance();
	primitiveDrawer->Initialize();
#pragma endregion

	// シーン切り替え
	Scene scene = TITLE;

	//タイトルシーンの初期化
	titleScene = new TitleScene();
	titleScene->Initialize();

	// ゲームシーンの初期化
	gameScene = new GameScene();
	gameScene->Initialize();

	//ゲームオーバーシーンの初期化
	gameoverScene = new GameOverScene();
	gameoverScene->Initialize();

	


	// メインループ
	while (true) {
		// メッセージ処理
		if (win->ProcessMessage()) {
			break;
		}

		// ImGui受付開始
		imguiManager->Begin();
		// 入力関連の毎フレーム処理
		input->Update();

		// 軸表示の更新
		axisIndicator->Update();
		


		// 描画開始
		dxCommon->PreDraw();

		//シーン遷移
		switch (scene) 
		{
		case TITLE:

			titleScene->Update();

			titleScene->Draw();
		
			scene = titleScene->GetScene();


			break;
		case GAME:


			// ゲームシーンの毎フレーム処理
			gameScene->Update();

			// ゲームシーンの描画
			gameScene->Draw();

			scene = gameScene->GetScene();

			
		
			

			break;
		case CLEAR:
			break;
		case OVER:



			gameoverScene->Update();
			gameoverScene->Draw();
			
		
			if (gameoverScene->GetState() == true)
			{
				return 0;
			}

			

			break;
		default:
			break;
		}

	

		// ImGui受付終了
		imguiManager->End();
	
		//// 軸表示の描画
		//axisIndicator->Draw();
		
		// プリミティブ描画のリセット
		primitiveDrawer->Reset();
		// ImGui描画
		imguiManager->Draw();
		// 描画終了
		dxCommon->PostDraw();
	}

	// 各種解放
	SafeDelete(gameScene);
	audio->Finalize();
	// ImGui解放
	imguiManager->Finalize();

	// ゲームウィンドウの破棄
	win->TerminateGameWindow();

	return 0;
}