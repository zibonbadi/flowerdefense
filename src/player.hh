#pragma once
#include "headers.hh"


enum class EPlayerMoveDirection {
	LEFT,
	RIGHT,
	UP,
	DOWN
};

enum class EPlayerAttackDirection {
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class Player {
	private:
		Sprite sprite;
		Animation* animations[8];
		bool walk_up = false, walk_down = false, walk_left = false, walk_right = false;
		SDL_FPoint delta				= { .x = 0, .y = 0 };
		EPlayerMoveDirection playerDir = EPlayerMoveDirection::LEFT;
		EPlayerAttackDirection attackDir = EPlayerAttackDirection::LEFT;
		EPlayerAttackDirection pastAttackDir;
		//Sprite* player, *attack;
		Sprite* player;

		Z_PlaneMeta* atk_collide = new Z_PlaneMeta{.w = 64.0f, .h = 64.0f};
		EBus_Fn *f_eHandler;
		Event
			*e_player_move_up, *e_player_move_down, *e_player_move_left, *e_player_move_right,
			*e_player_attack_up, * e_player_attack_down, * e_player_attack_left, * e_player_attack_right,
			*e_player_place_fence
			;
		float damageAnimCooldown = 0;
	public:
		Sprite *attack;
		float playerSpeed = 200.f;
		SDL_FPoint playerCoordinates	= { .x = 0, .y = 0 };
		unsigned int health = 4, obstacles = 4;
		int xp_bar = 200;
		int xp_bekommt = 100;
		int akt_LV = 0;


		Player(float x, float y);
		void initAnimations();
		void initControls();
		void ChangePlayerAnimation(const std::string animIDadditional);
		void Update(const float& deltaTime, const std::vector<Enemy*> &enemies);
		void reset(float x, float y);
		Sprite* GetSprite();
		void handleEvents(Event* e);
		~Player();
		std::string _animID;

		//int leben = 0;
};

