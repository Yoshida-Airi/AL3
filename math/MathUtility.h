#pragma once
#include <Matrix4x4.h>
#include <Vector3.h>
#include <assert.h>
#include <cmath>
#include <math.h>

// 加算
Vector3 Add(const Vector3& v1, const Vector3& v2);

// 減算
Vector3 Subtract(const Vector3& v1, const Vector3& v2);

// スカラー倍
Vector3 Multiply(float scalar, const Vector3& v);
// 内積
float Dot(const Vector3& v1, const Vector3& v2);

// 長さ(ノルム)
float Length(const Vector3& v);

// 正規化
Vector3 Normalize(const Vector3& v);

// 1.行列の加法
Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);

// 2.行列の減法
Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2);

// 行列の掛け算の関数
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

// 4.逆行列
Matrix4x4 Inverse(const Matrix4x4& m);
////5.転置行列
Matrix4x4 Transpose(const Matrix4x4& m);

////6.単位行列の作成
Matrix4x4 MakeIdentity4x4();

// 平行移動(translate)
// 平行移動行列の関数
Matrix4x4 MakeTranselateMatrix(const Vector3& transelate);

// 拡大縮小(scale)
// 拡大縮小行列の関数
Matrix4x4 MakeScaleMatrix(const Vector3& scale);

// 3.座標変換
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);



// 回転(rotate)
// 全軸の回転関数
Matrix4x4 MakeRotateMatrix(const Vector3& radian);


// x軸回転行列の関数
Matrix4x4 MakeRotateXMatrix(float radian);

// y軸回転行列の関数
Matrix4x4 MakeRotateYMatrix(float radian);

// z軸回転行列の関数
Matrix4x4 MakeRotateZMatrix(float radian);

// 3次元アフィン変換行列の関数
Matrix4x4 MakeAffinMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

// 1.透視投影行列
Matrix4x4 MakePerspectiveFovMatrix(float forY, float aspectRatio, float nearClip, float farClip);
// 2.正射影行列
Matrix4x4 MakeOrthographicmatrix(
    float left, float top, float right, float bottom, float nearClip, float farClip);

// 3.ビューポート変換行列
Matrix4x4 MakeViewportMatrix(
    float left, float top, float width, float height, float minDepth, float maxDepth);


// Vector3の計算
Vector3 SumVector3(Vector3& num1, Vector3& num2);

// 線形補間
Vector3 Lerp(const Vector3& s, const Vector3& e, float t);

float Lerp(const float& a, const float& b, float t);

// 球面線形補間
Vector3 Slerp(const Vector3& v1, const Vector3& v2, float t);

//最短角度補間
float LerpShortAngle(float a, float b, float t);