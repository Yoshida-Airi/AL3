#pragma once
#include <Matrix4x4.h>
#include <Vector3.h>
#include <assert.h>
#include <cmath>

class WorldTransform {
private:
	// 行列の掛け算の関数
	Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

	// 平行移動(translate)
	// 平行移動行列の関数
	Matrix4x4 MakeTranselateMatrix(const Vector3& transelate);

	// 拡大縮小(scale)
	// 拡大縮小行列の関数
	Matrix4x4 MakeScaleMatrix(const Vector3& scale);
	// 回転(rotate)

	// x軸回転行列の関数
	Matrix4x4 MakeRotateXMatrix(float radian);

	// y軸回転行列の関数
	Matrix4x4 MakeRotateYMatrix(float radian);

	// z軸回転行列の関数
	Matrix4x4 MakeRotateZMatrix(float radian);

	// 3次元アフィン変換行列の関数
	Matrix4x4
	    MakeAffinMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

	// スケール、回転、平行移動を合成して行列を計算する
	void UpdateMatrix();
};