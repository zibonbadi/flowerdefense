#ifndef SPRITE_H
#define SPRITE_H

#include "headers.h"

class Sprite {
	// Visual plane for internal grephics model
	
	private:
		// Bounds spec:
		// MSB = Inner bounds
		// LSB = Outer bounds
		// See defs.h -> tilebound for flags
		Z_PlaneMeta bounds;
		uint8_t bounds_mask = 0b00000000;
		//bool locked_transform = false; // Prevent transformations (screen buffers)
		Z_PlaneMeta transform_rect;
		std::map<std::string, Animation*> anime; // Animations (for later)
		std::string current_anime; // Selected for rendering

	public:
		Sprite();
		Sprite(std::string first_anim_name, Animation* first_anim, Z_PlaneMeta init);
		Sprite(Z_PlaneMeta init);
		~Sprite();
		int switch_to(std::string animation);
		int add_animation(std::string name, Animation* animation);
		void transform(Z_PlaneMeta add_tf);
		Z_PlaneMeta get_transform();
		void setTransform(Z_PlaneMeta new_tf);
		void render(SDL_Renderer* renderer);
		void render(SDL_Renderer *renderer, Z_PlaneMeta transform);
};
#endif

