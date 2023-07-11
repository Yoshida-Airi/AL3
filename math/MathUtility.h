#pragma once
#include <Matrix4x4.h>
#include <Vector3.h>
#include <assert.h>
#include <cmath>
#include <math.h>

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
Matrix4x4 MakeAffinMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

// Vector3の計算
Vector3 SumVector3(Vector3& num1, Vector3& num2);

// 長さ(ノルム)
float Length(const Vector3& v);

// 正規化
Vector3 Normalize(const Vector3& v);
