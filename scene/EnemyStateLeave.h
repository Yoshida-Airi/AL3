#pragma once
#include"IEnemyState.h"


class EnemyStateLeave : public BaseEnemyState
{
public:
	void Initialize(Enemy* pEnmey);
	void Update(Enemy* pEnemy);
};
