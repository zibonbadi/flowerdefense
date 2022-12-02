#pragma once
#include "headers.hh"
#include "enemy.hh"
class Enemypool
{
private:
	Animation* animations[9];
public:
	std::vector<Enemy*> enemies;
	void initAnimations();
	Enemypool(int count);
	// Globally store animations to save redundancy
};

