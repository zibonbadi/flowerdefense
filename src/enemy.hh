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
		ResourceManager& _rc;
		EventBus& _eb;
		Sprite sprite;
		Animation* animations[8];
		float enemySpeed					= 0.5f;
		SDL_Point goalTileCoordinates		= { .x = 0, .y = 0 };
		SDL_FPoint interpolStepSize			= { .x = 0, .y = 0 };
		EEnemyDirection enemyDir;
		Sprite* enemy;
	public:
		SDL_FPoint coordinates				= { .x = 0, .y = 0 };
		Enemy(float x, float y, ResourceManager& rc, EventBus& eb);
		void initAnimations();
		void Update(Tilemap* bfsArrows);
		Sprite* GetSprite();
		~Enemy();
};
