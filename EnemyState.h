#pragma once
#include"Vector3.h"

class Enemy; // 前方宣言

class BaseEnemyState {

public:
	// 仮想デストラクタ
	virtual ~BaseEnemyState(){};

	// 純粋仮想関数
	virtual void update(Enemy* pEnemy) = 0;
};

// 接近
class EnemyStateApproach : public BaseEnemyState {
public:
	void update(Enemy* pEnemy);
	Vector3 ApprochVelocity_ = {0, 0, -0.5f};

};

// 離脱
class EnemyStateLeave : public BaseEnemyState {
public:
	void update(Enemy* pEnemy);
	Vector3 LeaveVelocity_ = {-0.5f, 0.5f, 0};
};
