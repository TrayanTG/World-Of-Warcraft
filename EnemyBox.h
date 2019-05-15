#pragma once
#include "Enemy.h"
#include "Box.h"

class EnemyBox: public Box, public Enemy
{
public:
	EnemyBox(char *tempBuff, const Enemy &enemy, int level);
	EnemyBox(const EnemyBox&) = delete;
};