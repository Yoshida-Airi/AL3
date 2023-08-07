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
#include"CollisionManager.h"

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

	//当たり判定処理
	CollisionManager* colliderManager_ = nullptr;

	// 自キャラ
	Player* player_ = nullptr;
	//敵キャラ
	Enemy* enemy_ = nullptr;
	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;
	//デバッグカメラ有効
	bool isDebugcameraActive_ = false;

	// テクスチャを読み込む
	uint32_t textureHandle_ = 0;

	// 3Dモデルデータ
	Model* model_ = nullptr;

	// ビュープロジェクション
	ViewProjection viewProjection_;

};