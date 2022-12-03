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
		Animation* animations[9];
		float enemySpeed					= 0.5f;
		SDL_Point goalTileCoordinates		= { .x = 0, .y = 0 };
		SDL_FPoint interpolStepSize			= { .x = 0, .y = 0 };
		EEnemyDirection enemyDir;
		Sprite* enemy;
		std::string _id;
		bool isdead = false;
	public:
		SDL_FPoint coordinates				= { .x = 0, .y = 0 };
		Enemy(float x, float y, std::string id);
		void init(float x, float y, std::string id);
		void initAnimations();
		void Update(Tilemap* bfsArrows);
		Sprite* GetSprite();
		void disappear();
		void dying();
		void reborn(float x, float y);
		~Enemy();
};

