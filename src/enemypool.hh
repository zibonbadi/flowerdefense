#pragma once
#include "headers.hh"
#include "enemy.hh"
class Enemypool
{
private:
	// Store animations in pool to save redundancy
	Animation* animations[9];
public:
	std::vector<Enemy*> enemies;
	Enemypool(int count);
	void initAnimations();
	void create();
	void destroy(Enemy* enemy);
};

