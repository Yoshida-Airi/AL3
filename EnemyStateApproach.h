#pragma once
#include"IEnemyState.h"
class EnemyStateApproach : public BaseEnemyState
{
public:
	void Initialize(Enemy* pEnmey);
	void Update(Enemy* pEnemy);
};
