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
		Sprite* enemy;
		std::string _id;
	public:
		SDL_FPoint coordinates				= { .x = 0, .y = 0 };
		Enemy(float x, float y);
		void init(float x, float y);
		void setSpriteAnimations();
		void Update(Tilemap* bfsArrows);
		Sprite* GetSprite();
		void disappear();
		void dying();
		void reborn(float x, float y);

		bool isdead = false;
		bool visible = true;

		//~Enemy();
};

