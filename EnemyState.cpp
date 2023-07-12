#include "EnemyState.h"

void EnemyStateApproach::update(Enemy* pEnemy, Vector3& velocity) {
	pEnemy->Move(velocity);
	pEnemy->ChangeState(new EnemyStateLeave());
}

void EnemyStateLeave::update(Enemy* pEnemy, Vector3& velocity) { pEnemy->Move(velocity); }

// 移動
void Enemy::Move(Vector3& velocity) {
	AffinMatrix_.SumVector3(worldTransform_.translation_, velocity);
}
