#include "GameScene.h"
#include "AxisIndicator.h"
#include "TextureManager.h"
#include <cassert>

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

	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("Sample.png");

	// 3Dモデルデータの生成
	model_ = Model::Create();
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	
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

	// 敵キャラの生成
	Enemy* enemy = new Enemy();
	// 敵キャラに自キャラのアドレスを渡す
	enemy->SetPlayer(player_);
	// 敵の初期化
	enemy->Initialize(model_, {20.0f, 2.0f, 50.0f});
	enemy->SetGameScene(this);
	//リストに登録
	enemys_.push_back(enemy);

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
	player_->Update();
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
	enemys_.remove_if([] (Enemy * enemy) {
		if (enemy->IsDead())
		{
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