#pragma once
#include "headers.hh"
#include "enemy.hh"

enum class EPlayerDirection {
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class Enemy;
class Player {
	private:
		Sprite sprite;
		Animation* animations[8];
		float playerSpeed = 300.f;
		SDL_FPoint playerCoordinates	= { .x = 0, .y = 0 };
		SDL_FPoint delta				= { .x = 0, .y = 0 };
		EPlayerDirection playerDir = EPlayerDirection::LEFT;
		EPlayerDirection pastPlayerDir;
		Sprite* player;
		EBus_Fn *f_eHandler;
		Event
			*e_player_up, *e_player_down, *e_player_left, *e_player_right,
			*e_player_fence_up, *e_player_fence_down, *e_player_fence_left, *e_player_fence_right
			;
		float damageAnimCooldown = 0;
	public:
		Player(float x, float y);
		void initAnimations();
		void initControls();
		void ChangePlayerAnimation(const std::string animIDadditional);
		void Update(const float& deltaTime, const std::vector<Enemy*> &enemies);
		Sprite* GetSprite();
		void handleEvents(Event* e);
		~Player();
		std::string _animID;

		int leben = -1;
};

