#pragma once
#include "headers.hh"

typedef enum
{
	LEFT,
	RIGHT,
	UP,
	DOWN
} EPlayerDirection;

class Player
{
	private:
		ResourceManager &_rc;
		EventBus& _eb;
		KeyMapper& _keymap;
		Sprite sprite;
		Animation* animations[4];
		float playerSpeed = 4.f;
		SDL_FPoint playerCoordinates	= { .x = 0, .y = 0 };
		SDL_FPoint delta				= { .x = 0, .y = 0 };
		EPlayerDirection playerDir = EPlayerDirection::LEFT;
		EPlayerDirection pastPlayerDir;
		Sprite *player;
		EBus_Fn f_set_dir;
		Event *e_player_up, *e_player_down, *e_player_left, *e_player_right;
	public:
		Player(float x, float y, ResourceManager &rc, EventBus &eb, KeyMapper &keymap);
		void initAnimations();
		void initControls();
		void Update();
		Sprite* GetSprite();
		~Player();
};

