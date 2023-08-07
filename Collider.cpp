#include "Collider.h"

void Collider::SetRadius(float radius)
{ radius_ = radius; }

// 衝突属性(自分)を指定
void Collider::SetCollisionAttribute(uint32_t attribute) 
{ collisionAttribute_ = attribute; }

// 衝突マスク(相手)を設定
void Collider::SetCollisionMask(uint32_t mask)
{ CollisionMask_ = mask; }