#pragma once
#include "headers.hh"

class Player
{
	private:
		ResourceManager &rm;
		Sprite sprite;
		Animation* animations[4];
	public:
		Player(ResourceManager &rc);
		~Player();
};

