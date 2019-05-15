#include "EnemyBox.h"

EnemyBox::EnemyBox(char *tempBuff, const Enemy &enemy, int level):
	Box(enemy.getName(), "", _itoa(level, tempBuff, 10), DEF_ENEMY_SIZE), Enemy(enemy)
{
	setLevel(level);
}