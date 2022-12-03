#pragma once
#include "headers.hh"
#include "enemy.hh"
class Enemypool
{
private:
	// Store animations in pool to save redundancy
	Animation* animations[9];
	Plane& _board;
	Z_PlaneMeta& _collide_enemy;
	EBus_Fn* f_eHandler;
public:
	int _poolSize = 1000;
	float _spawnTime = 5.f;
	int _spawnCount = 3;
	float _spawnTimer = _spawnTime;
	int _availableEnemiesSize;
	std::vector<Enemy*> enemies;
	std::vector<Enemy*> availableEnemies;
	Enemypool(Plane& board, Z_PlaneMeta& collide_enemy, float spawnTime, int spawnCount, int poolSize);
	~Enemypool();
	void Update(const float& deltaTime);
	void Spawn(int count);
	void initAnimations();
	void create();
	void destroy(Enemy* enemy);
	void handleEvents(Event* e);
};
