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
	delete modelSkydome_;

	// 自キャラの解放
	delete player_;
	// 敵キャラの解放
	for (Enemy* enemy : enemys_)
	{
		delete enemy;
	}
	//敵弾の解放
	for (EnemyBullet* bullet : enemyBullets_) {
		delete bullet;
	}

	//天球の解放
	delete skydome_;
	//レールカメラの解放
	delete railCamera_;

}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// 敵のデータ取得
	LoadEnemyPopData();


	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("Sample.png");


	// 3Dモデルデータの生成
	model_ = Model::Create();
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	//レティクルのテクスチャ
	TextureManager::Load("reticle.png");

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	// 自キャラの生成
	player_ = new Player();
	
	//天球の生成
	skydome_ = new Skydome();
	//レールカメラの生成
	railCamera_ = new RailCamera();

	
	// レールカメラの初期化
	railCamera_->Initialize(worldTransform_, worldTransform_.rotation_);
	// 自キャラの初期化
	Vector3 playerPosition(0, 0, 30);
	player_->Initialize(model_, textureHandle_, playerPosition);

	

	//天球の初期化
	skydome_->Initialize(modelSkydome_, {0, 0, 0});

	//自キャラとレールカメラの親子関係を結ぶ
	player_->SetParent(&railCamera_->GetWorldTransform());

	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
}

void GameScene::Update() {


	viewProjection_.UpdateMatrix();


	// 自キャラの更新
	player_->Update(viewProjection_);

	//敵の生成
	UpdateEnemyPopCommands();

	// 敵キャラの更新
	for (Enemy* enemy : enemys_) {
		enemy->Update();
	}

	// 敵弾の更新
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Update();
	}

	// 当たり判定
	ChackAllCollisions();

	// デスフラグの立った敵を削除
	enemys_.remove_if([](Enemy* enemy) {
		if (enemy->IsDead()) {
			delete enemy;
			return true;
		}
		return false;
	});

	// デスフラグの立った弾を削除
	enemyBullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	//天球の更新
	skydome_->Update();


	//レールカメラの更新
	railCamera_->Update();
	viewProjection_.matView = railCamera_->GetViewProjection().matView;
	viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
	viewProjection_.TransferMatrix();

}

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) {
	// リストに登録する
	enemyBullets_.push_back(enemyBullet);
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

	// 敵キャラ
	for (Enemy* enemy : enemys_) {
		enemy->Draw(viewProjection_);
	}

	//敵弾
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Draw(viewProjection_);
	}
	//天球
	skydome_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	player_->DrawUI();


	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::SpawnEnemy(const Vector3& position)
{
	// 敵を発生させる
	Enemy* enemy = new Enemy();
	// 敵キャラに自キャラのアドレスを渡す
	enemy->SetPlayer(player_);
	enemy->SetGameScene(this);
	// 敵の初期化
	enemy->Initialize(model_, position);

	// リストに登録
	enemys_.push_back(enemy);
}

/// 敵発生データの読み込み
void GameScene::LoadEnemyPopData()
{
	//ファイルを開く
	std::ifstream file;
	file.open("csv/enemyPop.csv");
	assert(file.is_open());

	//ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	//ファイルを閉じる
	file.close();
}

/// 敵発生コマンドの更新
void GameScene::UpdateEnemyPopCommands()
{
	//待機処理
	if (isWaitTime_)
	{
		waitTimer_--;
		if (waitTimer_ <= 0)
		{
			//待機完了
			isWaitTime_ = false;
		}
		return;
	}

	//1行分の文字列を入れる変数
	std::string line;

	// コマンド実行ループ
	while (getline(enemyPopCommands, line)) {
		// 一桁分の文字列をストリームに変換して解析しやすくなる
		std::istringstream line_stream(line);

		std::string word;
		//,区切りで行の先端文字列を取得
		getline(line_stream, word, ',');

		//"//"から始まる行はコメント
		if (word.find("//") == 0) {
			// コメント行を飛ばす
			continue;
		}

		// POPコマンド
		if (word.find("POP") == 0) {
			// x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			// 敵を発生させる
			SpawnEnemy(Vector3(x, y, z));
		}

		// WAITコマンド
		else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			// 待ち時間
			int32_t waitTime = atoi(word.c_str());

			// 待機開始
			isWaitTime_ = true;
			waitTimer_ = waitTime;

			// コマンドループを抜ける
			break;
		}
	}
}

/// 衝突判定と応答
void GameScene::ChackAllCollisions() {
	// 判定対象AとBの座標
	Vector3 posA, posB;

	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();

	//半径
	const float radius = 2.0f;

#pragma region 自キャラと敵弾の当たり判定

	//自キャラの座標
	posA = player_->GetWorldPosition();

	//自キャラと敵弾全ての当たり判定
	for (EnemyBullet* bullet : enemyBullets_) {
		// 敵弾の座標
		posB = bullet->GetWorldPosition();

		//座標AとBの距離を求める
		Vector3 distance = {posB.x - posA.x, posB.y - posA.y, posB.z - posA.z};

		//球同士が当たっていれば
		if ((distance.x*distance.x) + (distance.y*distance.y)+ (distance.z* distance.z) <= (radius* radius))
		{
			//自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			//敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}

#pragma endregion

#pragma region 自弾と敵キャラの当たり判定

	//敵キャラの座標

	//自弾と敵キャラの全ての当たり判定
	for (Enemy* enemy : enemys_) {
		for (PlayerBullet* bullet : playerBullets) {
			// 自弾の座標
			posA = enemy->GetWorldPosition();
			posB = bullet->GetWorldPosition();

			// 座標AとBの距離を求める
			Vector3 distance = {posB.x - posA.x, posB.y - posA.y, posB.z - posA.z};

			// 球同士が当たっていれば
			if ((distance.x * distance.x) + (distance.y * distance.y) + (distance.z * distance.z) <=
			    (radius * radius)) {
				// 敵キャラの衝突時コールバックを呼び出す
				enemy->OnCollision();
				// 自弾の衝突時コールバックを呼び出す
				bullet->OnCollision();
			}
		}
	}

#pragma endregion

#pragma region 自弾と敵弾の当たり判定

	for (PlayerBullet* playerBullet : playerBullets)
	{
		for (EnemyBullet* enemyBullet : enemyBullets_)
		{
			//自弾の座標
			posA = playerBullet->GetWorldPosition();
			//敵弾の座標
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