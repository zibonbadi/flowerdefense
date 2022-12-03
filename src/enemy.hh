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

class Enemy {
	private:
		Sprite sprite;
		float enemySpeed					= 0.5f;
		SDL_Point goalTileCoordinates		= { .x = 0, .y = 0 };
		SDL_FPoint interpolStepSize			= { .x = 0, .y = 0 };
		EEnemyDirection enemyDir;
		std::string _id;
		Player& _player;
	public:
		SDL_FPoint coordinates				= { .x = 0, .y = 0 };
		Enemy(float x, float y, Player& player);
		void init(float x, float y);
		void setSpriteAnimations();
		void Update(const BFS& bfsFlower,const BFS& bfsPlayer);
		Sprite* GetSprite();
		void disappear();
		void dying();
		void reborn(float x, float y);

		bool isdead = false;

		//~Enemy();
};

