#pragma once
#include "headers.hh"
#include "enemy.hh"
class Enemypool
{
public:
	std::vector<Enemy*> enemies;
	Enemypool(int count);
};

