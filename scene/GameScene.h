#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Player.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include"DebugCamera.h"
#include"Enemy.h"
#include"Skydome.h"
#include"RailCamera.h"
#include"WorldTransform.h"
#include"EnemyBullet.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 敵弾を追加する
	/// </summary>
	/// <param name="enemyBullet">敵弾</param>
	void AddEnemyBullet(EnemyBullet* enemyBullet);

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	// 自キャラ
	Player* player_ = nullptr;
	//天球
	Skydome* skydome_ = nullptr;
	//レールカメラ
	RailCamera* railCamera_ = nullptr;

	//敵リスト
	std::list<Enemy*> enemys_;

	//敵弾リスト
	std::list<EnemyBullet*> enemyBullets_;

	////デバッグカメラ
	//DebugCamera* debugCamera_ = nullptr;
	////デバッグカメラ有効
	//bool isDebugcameraActive_ = false;

	// テクスチャを読み込む
	uint32_t textureHandle_ = 0;

	// 3Dモデルデータ
	Model* model_ = nullptr;
	Model* modelSkydome_ = nullptr;

	//ワールドトランスフォーム
	WorldTransform worldTransform_;
	// ビュープロジェクション
	ViewProjection viewProjection_;

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void ChackAllCollisions();

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};