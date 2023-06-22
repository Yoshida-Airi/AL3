#pragma once
#include <Vector.h>
#include<WorldTransform.h>
#include <WorldAffinMatrix.h>
#include <assert.h>
#include <cmath>
#include <string>


// 行列の掛け算の関数
Matrix4x4 WorldTransformEX::Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 resultMultiply;
	for (int row = 0; row < 4; ++row) {
		for (int column = 0; column < 4; ++column) {
			resultMultiply.m[row][column] =
			    m1.m[row][0] * m2.m[0][column] + m1.m[row][1] * m2.m[1][column] +
			    m1.m[row][2] * m2.m[2][column] + m1.m[row][3] * m2.m[3][column];
		}
	}
	return resultMultiply;
}

// 平行移動(translate)
// 平行移動行列の関数
Matrix4x4 WorldTransformEX::MakeTranselateMatrix(const Vector3& transelate) {
	Matrix4x4 result;
	result.m[0][0] = 1.0f;
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = 1.0f;
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = 1.0f;
	result.m[2][3] = 0.0f;

	result.m[3][0] = transelate.x;
	result.m[3][1] = transelate.y;
	result.m[3][2] = transelate.z;
	result.m[3][3] = 1.0f;

	return result;
}

// 拡大縮小(scale)
// 拡大縮小行列の関数
Matrix4x4 WorldTransformEX::MakeScaleMatrix(const Vector3& scale) {
	Matrix4x4 result;
	result.m[0][0] = scale.x;
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = scale.y;
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = scale.z;
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return result;
}

// 回転(rotate)

// x軸回転行列の関数
Matrix4x4 WorldTransformEX::MakeRotateXMatrix(float radian) {
	Matrix4x4 resultMakeRotatedMatrix;
	resultMakeRotatedMatrix.m[0][0] = 1;
	resultMakeRotatedMatrix.m[0][1] = 0;
	resultMakeRotatedMatrix.m[0][2] = 0;
	resultMakeRotatedMatrix.m[0][3] = 0;

	resultMakeRotatedMatrix.m[1][0] = 0;
	resultMakeRotatedMatrix.m[1][1] = std::cos(radian);
	resultMakeRotatedMatrix.m[1][2] = std::sin(radian);
	resultMakeRotatedMatrix.m[1][3] = 0;

	resultMakeRotatedMatrix.m[2][0] = 0;
	resultMakeRotatedMatrix.m[2][1] = -std::sin(radian);
	resultMakeRotatedMatrix.m[2][2] = std::cos(radian);
	resultMakeRotatedMatrix.m[2][3] = 0;

	resultMakeRotatedMatrix.m[3][0] = 0;
	resultMakeRotatedMatrix.m[3][1] = 0;
	resultMakeRotatedMatrix.m[3][2] = 0;
	resultMakeRotatedMatrix.m[3][3] = 1;

	return resultMakeRotatedMatrix;
}

// y軸回転行列の関数
Matrix4x4 WorldTransformEX::MakeRotateYMatrix(float radian) {
	Matrix4x4 resultMakeRotatedMatrix;
	resultMakeRotatedMatrix.m[0][0] = std::cos(radian);
	resultMakeRotatedMatrix.m[0][1] = 0;
	resultMakeRotatedMatrix.m[0][2] = -std::sin(radian);
	resultMakeRotatedMatrix.m[0][3] = 0;

	resultMakeRotatedMatrix.m[1][0] = 0;
	resultMakeRotatedMatrix.m[1][1] = 1;
	resultMakeRotatedMatrix.m[1][2] = 0;
	resultMakeRotatedMatrix.m[1][3] = 0;

	resultMakeRotatedMatrix.m[2][0] = std::sin(radian);
	resultMakeRotatedMatrix.m[2][1] = 0;
	resultMakeRotatedMatrix.m[2][2] = std::cos(radian);
	resultMakeRotatedMatrix.m[2][3] = 0;

	resultMakeRotatedMatrix.m[3][0] = 0;
	resultMakeRotatedMatrix.m[3][1] = 0;
	resultMakeRotatedMatrix.m[3][2] = 0;
	resultMakeRotatedMatrix.m[3][3] = 1;

	return resultMakeRotatedMatrix;
}

// z軸回転行列の関数
Matrix4x4 WorldTransformEX::MakeRotateZMatrix(float radian) {
	Matrix4x4 resultMakeRotatedMatrix;
	resultMakeRotatedMatrix.m[0][0] = std::cos(radian);
	resultMakeRotatedMatrix.m[0][1] = std::sin(radian);
	resultMakeRotatedMatrix.m[0][2] = 0;
	resultMakeRotatedMatrix.m[0][3] = 0;

	resultMakeRotatedMatrix.m[1][0] = -std::sin(radian);
	resultMakeRotatedMatrix.m[1][1] = std::cos(radian);
	resultMakeRotatedMatrix.m[1][2] = 0;
	resultMakeRotatedMatrix.m[1][3] = 0;

	resultMakeRotatedMatrix.m[2][0] = 0;
	resultMakeRotatedMatrix.m[2][1] = 0;
	resultMakeRotatedMatrix.m[2][2] = 1;
	resultMakeRotatedMatrix.m[2][3] = 0;

	resultMakeRotatedMatrix.m[3][0] = 0;
	resultMakeRotatedMatrix.m[3][1] = 0;
	resultMakeRotatedMatrix.m[3][2] = 0;
	resultMakeRotatedMatrix.m[3][3] = 1;

	return resultMakeRotatedMatrix;
}

// 3次元アフィン変換行列の関数
Matrix4x4 WorldTransformEX::MakeAffinMatrix(
    const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	Matrix4x4 resultMakeAffinMatrix;
	Matrix4x4 resultMakeScaleMatrix = MakeScaleMatrix(scale);
	Matrix4x4 resultMakeTranselateMatrix = MakeTranselateMatrix(translate);
	Matrix4x4 resultMakeRotateXMatrix = MakeRotateXMatrix(rotate.x);
	Matrix4x4 resultMakeRotateYMatrix = MakeRotateYMatrix(rotate.y);
	Matrix4x4 resultMakeRotateZMatrix = MakeRotateZMatrix(rotate.z);

	Matrix4x4 rotateXYZMatrix = Multiply(
	    resultMakeRotateXMatrix, Multiply(resultMakeRotateYMatrix, resultMakeRotateZMatrix));

	resultMakeAffinMatrix =
	    Multiply(resultMakeScaleMatrix, Multiply(rotateXYZMatrix, resultMakeTranselateMatrix));

	return resultMakeAffinMatrix;
}

Vector3 WorldTransformEX::TransformNormal(const Vector3& v, const Matrix4x4& m) {
	Vector3 result =
	{
	    v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0],
	    v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1],
	    v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2],
	};

	return result;
}

Vector3 WorldTransformEX::SumVector3(Vector3& num1, Vector3& num2) {
	Vector3 result{};
	result.x = num1.x += num2.x;
	result.y = num1.y += num2.y;
	result.z = num1.z += num2.z;
	return result;
}


// 長さ(ノルム)
float WorldTransformEX::Length(const Vector3& v) {
	float result;
	result = powf(v.x, 2.0) + powf(v.y, 2.0) + powf(v.z, 2.0);

	return sqrtf(result);
};

// 正規化
Vector3 WorldTransformEX::Normalize(const Vector3& v) {
	Vector3 result;
	float x;
	x = Length(v);
	result.x = v.x / x;
	result.y = v.y / x;
	result.z = v.z / x;
	return result;
}