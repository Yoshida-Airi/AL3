#pragma once
#include"Vector3.h"

class Enemy; // 前方宣言

class BaseEnemyState {
protected:
	Enemy* enemy_ = {0};

public:
	// 仮想デストラクタ
	virtual ~BaseEnemyState(){};

	// 純粋仮想関数
	virtual void update(Enemy* pEnemy, Vector3& velocity) = 0;
};

// 接近
class EnemyStateApproach : public BaseEnemyState {
public:
	void update(Enemy* pEnemy, Vector3& velocity);
};

// 離脱
class EnemyStateLeave : public BaseEnemyState {
public:
	void update(Enemy* pEnemy, Vector3& velocity);
};
