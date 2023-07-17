#pragma once
#include"WorldTransform.h"
#include"ViewProjection.h"
#include"MathUtility.h"
#include"ImGuiManager.h"

/// <summary>
/// レールカメラ
/// </summary>
class RailCamera 
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(WorldTransform worldTransform, Vector3& radian);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// ビュープロジェクションのゲッター
	/// </summary>
	/// <returns>レールカメラのビュープロジェクション</returns>
	ViewProjection GetViewProjection() { return viewProjection_; };

	/// <summary>
	/// トランスフォームのゲッター
	/// </summary>
	/// <returns>レールカメラのワールドトランスフォーム</returns>
	const WorldTransform& GetWorldTransform() const { return worldTransform_; };


private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	//ビュープロジェクション
	ViewProjection viewProjection_;

};
