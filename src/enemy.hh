#pragma once
#include "headers.hh"

enum class EEnemyDirection {
	TOP,
	BOTTOM,
	LEFT,
	RIGHT,
	TOPLEFT,
	BOTTOMRIGHT,
	BOTTOMLEFT,
	TOPRIGHT
};

enum class EEnemyType {
	BUG,
	MEALWORM,
	BEE
};

class Enemy {
	private:
		Sprite sprite;
		
		SDL_Point goalTileCoordinates		= { .x = 0, .y = 0 };
		SDL_FPoint interpolStepSize			= { .x = 0, .y = 0 };
		EEnemyDirection _enemyDir			= EEnemyDirection::RIGHT;
		std::string _id;
		Player& _player;
		std::random_device rd;
		std::mt19937* g = new std::mt19937(rd());  // rd is a std::random_device object
		std::uniform_int_distribution<int>* dist5;

	public:
		EEnemyType _enemyType;
		SDL_FPoint coordinates				= { .x = 0, .y = 0 };
		float _flightPathRoseLength			= INFINITY;
		Enemy(float x, float y, Player& player, EEnemyType enemyType, EEnemyDirection enemyDir);
		void init(float x, float y);
		void setSpriteAnimations();
		void Update(const BFS& bfsFlower,const BFS& bfsPlayer,Tilemap *course, float deltaTime);
		Sprite* GetSprite();
		void disappear();
		void dying();
		void reborn(float x, float y);

		bool isdead = false;
		bool visible = false;

		float _enemySpeed;

		//~Enemy();
};

