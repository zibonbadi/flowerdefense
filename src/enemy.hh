#pragma once
#include "headers.hh"

enum class EEnemyDirection {
	LEFT,
	RIGHT,
	TOP,
	BOTTOM,
	TOPLEFT,
	TOPRIGHT,
	BOTTOMLEFT,
	BOTTOMRIGHT
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
		EEnemyDirection enemyDir;
		std::string _id;
		Player& _player;
	public:
		EEnemyType _enemyType;
		SDL_FPoint coordinates				= { .x = 0, .y = 0 };
		Enemy(float x, float y, Player& player, EEnemyType enemyType);
		void init(float x, float y);
		void setSpriteAnimations();
		void Update(const BFS& bfsFlower,const BFS& bfsPlayer);
		Sprite* GetSprite();
		void disappear();
		void dying();
		void reborn(float x, float y);

		bool isdead = false;
		bool visible = false;

		float _enemySpeed;

		//~Enemy();
};

