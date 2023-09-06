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
#include"Ground.h"
#include"FollowCamera.h"

#include "ViewProjection.h"
#include "WorldTransform.h"

#include<sstream>
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



private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	// 自キャラ
	Player* player_ = nullptr;
	
	//カメラ
	FollowCamera* camera_ = nullptr;
	

	////デバッグカメラ
	// DebugCamera* debugCamera_ = nullptr;
	////デバッグカメラ有効
	// bool isDebugcameraActive_ = false;

	// テクスチャを読み込む
	uint32_t textureHandle_ = 0;


	// 3Dモデルデータ
	Model* model_ = nullptr;

	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	// ビュープロジェクション
	ViewProjection viewProjection_;


	bool isWaitTime_ = true;	//待機中フラグ　true::待機中
	int32_t waitTimer_ = 0;	//待機タイマー


private:
	///// <summary>
	///// 衝突判定と応答
	///// </summary>
	//void ChackAllCollisions();

	
};