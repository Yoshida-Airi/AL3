#pragma once

#include "Audio.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Enemy.h"
#include "EnemyBullet.h"
#include "Input.h"
#include "Model.h"
#include "Player.h"
#include "RailCamera.h"
#include "SafeDelete.h"
#include "Skydome.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "WorldTransform.h"
#include<sstream>

#include"SceneState.h"

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

	
	/// <summary>
	/// 敵の発生
	/// </summary>
	void SpawnEnemy(const Vector3& position);

	/// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	void LoadEnemyPopData();

	/// <summary>
	/// 敵発生コマンドの更新
	/// <summary>
	void UpdateEnemyPopCommands();

	Scene GetScene() const { return scene_; };

	void Reset();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	// 自キャラ
	Player* player_ = nullptr;
	// 天球
	Skydome* skydome_ = nullptr;
	// レールカメラ
	RailCamera* railCamera_ = nullptr;

	// 敵リスト
	std::list<Enemy*> enemys_;

	// 敵弾リスト
	std::list<EnemyBullet*> enemyBullets_;

	

	////デバッグカメラ
	// DebugCamera* debugCamera_ = nullptr;
	////デバッグカメラ有効
	// bool isDebugcameraActive_ = false;

	// テクスチャを読み込む
	uint32_t textureHandle_ = 0;
	

	// 3Dモデルデータ
	Model* model_ = nullptr;
	Model* modelSkydome_ = nullptr;
	Model* modelPlayer_ = nullptr;
	Model* modelPlayerBullet_ = nullptr;
	Model* modelEnemyBullet_ = nullptr;
	Model* modelEnemy_ = nullptr;

	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	// ビュープロジェクション
	ViewProjection viewProjection_;

	// 敵発生コマンド
	std::stringstream enemyPopCommands;

	bool isWaitTime_ = true;	//待機中フラグ　true::待機中
	int32_t waitTimer_ = 0;	//待機タイマー

	Scene scene_;

	bool playerAlive_ = true;

	uint32_t soundData2_ = 0;

	bool sceneState_ = false;

	bool ifAudio = false;

private:
	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void ChackAllCollisions();

	
};