#include "GameScene.h"
#include "AxisIndicator.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	// モデルの開放
	delete model_;
	delete modelSkydome_;

	// 自キャラの解放
	delete player_;
	// 敵キャラの解放
	delete enemy_;
	// 天球の解放
	delete skydome_;

	// デバッグカメラ
	delete debugCamera_;

	// 雨の解放
	for (Rain* rain : rains_) {
		delete rain;
	}
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	// 自キャラの生成
	player_ = new Player();
	// 敵キャラの生成
	enemy_ = new Enemy();
	// 天球の生成
	skydome_ = new Skydome();

	// 敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);

	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("Sample.png");

	// 3Dモデルデータの生成
	model_ = Model::Create();
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	modelRain_ = Model::CreateFromOBJ("Rain", true);

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	// ビュープロジェクションの初期化
	debugCamera_->SetFarZ(1000);


	viewProjection_.Initialize();

	// 自キャラの初期化
	player_->Initialize(model_, textureHandle_);
	// 敵の初期化
	enemy_->Initialize(model_, {20.0f, 2.0f, 50.0f});
	// 天球の初期化
	skydome_->Initialize(modelSkydome_, {0, 0, 0});

	rains_.clear();

	for (int i = 0; i < kNumParticles; i++) {
		// 雨の生成
		Rain* newRain = new Rain();
		// 雨の初期化
		newRain->Initialize(modelRain_, {0, 30, 0});

		newRain->SetPlayer(player_);
		
		// 雨を登録する
		rains_.push_back(newRain);
	}

	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
}

void GameScene::Update() {

	ChackAllCollisions();

	// 自キャラの更新
	player_->Update();
	// 敵キャラの更新
	enemy_->Update();
	// 天球の更新
	skydome_->Update();

	// 雨の更新
	for (Rain* rain : rains_) {
		rain->Update();
	}

	// デバッグカメラの更新
	debugCamera_->Update();

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_C))
	{
		isDebugcameraActive_ = true;
	}
	// カメラの処理
	if (isDebugcameraActive_) {

		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;

		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();

	} else
	{
		// ビュープロジェクション行列の更新と転送
		viewProjection_.UpdateMatrix();
	}

#endif
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

	// 自キャラの描画
	player_->Draw(viewProjection_);
	// 敵キャラの描画
	enemy_->Draw(viewProjection_);
	// 天球の描画
	skydome_->Draw(viewProjection_);
	// 雨の描画
	for (Rain* rain : rains_) {
		rain->Draw(viewProjection_);
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

/// 衝突判定と応答
void GameScene::ChackAllCollisions() {
	// 判定対象AとBの座標
	Vector3 posA, posB;

	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullet();

	// 半径
	const float radius = 2.0f;

#pragma region 自キャラと敵弾の当たり判定

	// 自キャラの座標
	posA = player_->GetWorldPosition();

	// 自キャラと敵弾全ての当たり判定
	for (EnemyBullet* bullet : enemyBullets) {
		// 敵弾の座標
		posB = bullet->GetWorldPosition();

		// 座標AとBの距離を求める
		Vector3 distance = {posB.x - posA.x, posB.y - posA.y, posB.z - posA.z};

		// 球同士が当たっていれば
		if ((distance.x * distance.x) + (distance.y * distance.y) + (distance.z * distance.z) <=
		    (radius * radius)) {
			// 自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			// 敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}

#pragma endregion

#pragma region 自弾と敵キャラの当たり判定

	// 敵キャラの座標
	posA = enemy_->GetWorldPosition();

	// 自弾と敵キャラの全ての当たり判定
	for (PlayerBullet* bullet : playerBullets) {
		// 自弾の座標
		posB = bullet->GetWorldPosition();

		// 座標AとBの距離を求める
		Vector3 distance = {posB.x - posA.x, posB.y - posA.y, posB.z - posA.z};

		// 球同士が当たっていれば
		if ((distance.x * distance.x) + (distance.y * distance.y) + (distance.z * distance.z) <=
		    (radius * radius)) {
			// 敵キャラの衝突時コールバックを呼び出す
			enemy_->OnCollision();
			// 自弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}

#pragma endregion

#pragma region 自弾と敵弾の当たり判定

	for (PlayerBullet* playerBullet : playerBullets) {
		for (EnemyBullet* enemyBullet : enemyBullets) {
			// 自弾の座標
			posA = playerBullet->GetWorldPosition();
			// 敵弾の座標
			posB = enemyBullet->GetWorldPosition();

			// 座標AとBの距離を求める
			Vector3 distance = {posB.x - posA.x, posB.y - posA.y, posB.z - posA.z};

			// 球同士が当たっていれば
			if ((distance.x * distance.x) + (distance.y * distance.y) + (distance.z * distance.z) <=
			    (radius * radius)) {
				// 敵キャラの衝突時コールバックを呼び出す
				playerBullet->OnCollision();
				// 自弾の衝突時コールバックを呼び出す
				enemyBullet->OnCollision();
			}
		}
	}

#pragma endregion
}