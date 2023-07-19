#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include"ImGuiManager.h"
#define _USE_MATH_DEFINES  
#include <math.h>
#include <cassert>
#include<list>
#include"player.h"

class Rain {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	void Initialize(Model* model, const Vector3& position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection"ビュープロジェクション（参照渡し）</param>
	void Draw(ViewProjection viewProjection);

	/// <summary>
	/// プレイヤーのインスタンスをセットする
	/// </summary>
	void SetPlayer(Player* player); 

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;



	// ImGuiで値を入力する変数
	float inputFloat3[3] = {0, 0, 0};

private:
	
	Player* player_;

	float mass_; // 質量
	float drawSize_;
	float size_;	//大きさ
	Vector3 velocity_;

	float airResistance_;	//空気抵抗


	float rainDensity_; // 雨粒の密度
	float airDensity_;  // 空気の密度
	float k_;           // 空気抵抗係数
	Vector3 kGravity_;  // 重力加速度

	Vector3 windSpeed_;	//風速
	
	
	Vector3 relativeVelocity_;

	bool isActive_ = false;
	bool isRelative_ = false;
};
