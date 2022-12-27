#pragma once
#include "headers.hh"

class Enemypool
{
private:
	// Store animations in pool to save redundancy
	Animation* animations[25];
	Plane& _board;
	Player& _player;
	Z_PlaneMeta& _collide_enemy;
	EBus_Fn* f_eHandler;
public:
	int _poolSize = 1000;
	float _spawnTime = 5.f;
	int _spawnCount = 3;
	float _spawnTimer = 0;//_spawnTime;
	int countOfAllSpawnedEnemies = 0;
	std::random_device rd;
	std::mt19937* g = new std::mt19937(rd());  // rd is a std::random_device object
	std::uniform_int_distribution<int>* dist;
	std::uniform_int_distribution<int>* dist2;
	std::uniform_int_distribution<int>* dist3;
	std::uniform_int_distribution<int>* dist4;
	//int _availableEnemiesSize;
	std::vector<Enemy*> enemies;
	//std::vector<Enemy*> availableEnemies;
	Enemypool(Plane& board, Player& player, Z_PlaneMeta& collide_enemy, float spawnTime, int spawnCount, int poolSize);
	~Enemypool();
	int getAvailableCount();
	Enemy* getFirstAvailable();
	void Update(const float& deltaTime, const int& waveNumber);
	void Recollect();
	void Spawn(int count, const int& waveNumber);
	void initAnimations();
	void create();
	void reset();
	void destroy(Enemy* enemy);
	void handleEvents(Event* e);
};
