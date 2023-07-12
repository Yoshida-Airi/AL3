#include "EnemyState.h"
#include"Enemy.h"

void EnemyStateApproach::update(Enemy* pEnemy) {
	pEnemy->Move(ApprochVelocity_);
	if (pEnemy->GetWorldTrans().z < 0.0f) {
		pEnemy->ChangeState(new EnemyStateLeave());
	}


}

void EnemyStateLeave::update(Enemy* pEnemy)
{ 
	pEnemy->Move(LeaveVelocity_);
}

