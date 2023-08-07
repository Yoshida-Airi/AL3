#include "GameScene.h"
#include "AxisIndicator.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	// 自キャラの解放
	delete player_;
	// 敵キャラの解放
	delete enemy_;
	// デバッグカメラ
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	// 自キャラの生成
	player_ = new Player();
	// 敵キャラの生成
	enemy_ = new Enemy();

	// 敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);

	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("Sample.png");

	// 3Dモデルデータの生成
	model_ = Model::Create();

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	// 自キャラの初期化
	player_->Initialize(model_, textureHandle_);
	// 敵の初期化
	enemy_->Initialize(model_, {20.0f, 2.0f, 50.0f});

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
}

void GameScene::Update() {

	// 自キャラの更新
	player_->Update();
	// 敵キャラの更新
	enemy_->Update();

	// 当たり判定
	ChackAllCollisions();

	// デバッグカメラの更新
	debugCamera_->Update();

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_C)) {
		isDebugcameraActive_ = true;
	}
	// カメラの処理
	if (isDebugcameraActive_) {
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {
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

	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullet();


	//コライダー
	std::list<Collider*> colliders_;

	//コライダーをリストに登録
	colliders_.push_back(player_);
	colliders_.push_back(enemy_);

	//自弾全てについて
	for (PlayerBullet* playerBullet : playerBullets)
	{
		colliders_.push_back(playerBullet);
	}

	//敵弾全てについて
	for (EnemyBullet* enemyBullet : enemyBullets)
	{
		colliders_.push_back(enemyBullet);
	}

	//リスト内のペアを総当たり
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA)
	{
		//イテレータAからコライダーAを取得する
		Collider* colliderA = *itrA;

		//イテレータBはイテレータAの次の要素から回す(重複判定を回避)

		std::list<Collider*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); ++itrB)
		{
			// イテレータBからコライダーBを取得する
			Collider* colliderB = *itrB;

			//ペアの当たり判定
			//コライダーAトコライダーBの当たり判定
			CheakCollisionPair(colliderA, colliderB);

		}
	}

}

void GameScene::CheakCollisionPair(Collider* colliderA, Collider* colliderB) {

	//衝突フィルタリング
	if (colliderA->GetCollisionAttribute() != colliderB->GetCollisionMask() ||
		colliderB->GetCollisionAttribute() != colliderA->GetCollisionMask())
	{
		return;
	}

	// コライダーAのワールド座標を取得
	Vector3 posA = colliderA->GetWorldPosition();
	// コライダーBのワールド座標を取得
	Vector3 posB = colliderB->GetWorldPosition();

	// 座標AとBの距離を求める
	Vector3 distance = {posB.x - posA.x, posB.y - posA.y, posB.z - posA.z};

	// 球と球の交差判定
	if ((distance.x * distance.x) + (distance.y * distance.y) + (distance.z * distance.z) <=
	    (colliderA->GetRadius() * colliderB->GetRadius())) {
		// コライダーAの衝突時コールバックを呼び出す
		colliderA->OnCollision();
		// コライダーBの衝突時コールバックを呼び出す
		colliderB->OnCollision();
	}
}