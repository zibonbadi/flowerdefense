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
	Hud&	_hud;
	int		_countOfAllSpawnedEnemies = 0;
	float	_spawnTimer = 0;//_spawnTime;
	float	_waveCoolDownTimer = -1.0f;
	int		_enemySpawnsInWaveLeftCounter;
	float	_adjustedSpawnTime;
	int		_adjustedEnemiesPerWaveCount;

public:
	const int   _poolSize = 1000;
	const int	_startSpawnTime = 2.f;
	const float _waveCoolDownTime = 6.0f;
	const float _spawnTimeDeltaAtWaveEnd = -0.2f;
	const float _spawnTimeMin = 0.2f;
	const int   _spawnCount = 1;
	const int	_startEnemiesPerWaveCount = 20;
	const float _enemiesPerWaveCountScalerAteWaveEnd = 1.3f;

	std::random_device rd;
	std::mt19937* g = new std::mt19937(rd());  // rd is a std::random_device object
	std::uniform_int_distribution<int>* dist;
	std::uniform_int_distribution<int>* dist2;
	std::uniform_int_distribution<int>* dist3;
	std::uniform_int_distribution<int>* dist4;
	//int _availableEnemiesSize;
	std::vector<Enemy*> enemies;
	//std::vector<Enemy*> availableEnemies;
	Enemypool(Plane& board, Player& player, Z_PlaneMeta& collide_enemy, Hud& hud);
	~Enemypool();
	int getAvailableCount();
	Enemy* getFirstAvailable();
	void Update(const float& deltaTime);
	int  getVisibleAndAliveCount();
	void Recollect();
	void Spawn(int count, const int& waveNumber);
	void initAnimations();
	void create();
	void reset();
	void destroy(Enemy* enemy);
	void handleEvents(Event* e);
};
