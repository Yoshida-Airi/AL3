#include "GameScene.h"
#include "AxisIndicator.h"
#include "TextureManager.h"
#include <cassert>
#include<iostream>
#include<fstream>
#include<string>

GameScene::GameScene() {}

GameScene::~GameScene() {
	//モデルの開放
	delete model_;

	// 自キャラの解放
	delete player_;
	
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();


	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("Sample.png");


	// 3Dモデルデータの生成
	model_ = Model::Create();

	//レティクルのテクスチャ
	TextureManager::Load("reticle.png");

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	// 自キャラの生成
	player_ = new Player();
	

	
	// 自キャラの初期化
	Vector3 playerPosition(0, 0, 30);
	player_->Initialize(model_, textureHandle_, playerPosition);

	
	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
}

void GameScene::Update() {


	viewProjection_.UpdateMatrix();


	// 自キャラの更新
	player_->Update(viewProjection_);



}



void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 自キャラ
	player_->Draw(viewProjection_);

	
	////天球
	//skydome_->Draw(viewProjection_);



	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	/*player_->DrawUI();*/


	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
