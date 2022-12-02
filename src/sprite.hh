#pragma once
#include "headers.hh"

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
		Z_PlaneMeta* box_collide = nullptr;
		Z_RGBA color = { .r = 0xFF, .g = 0x00, .b = 0xFF };
		std::map<std::string, Animation*> anime; // Animations (for later)
		std::string current_anime; // Selected for rendering

	public:
		Sprite();
		Sprite(std::string first_anim_name, Animation* first_anim, Z_PlaneMeta init);
		Sprite(Z_PlaneMeta init);
		~Sprite();

		// Render settings
		int switch_to_anim(std::string animation);
		int set_color(Z_RGBA color);
		int add_animation(std::string name, Animation* animation);
		void transform(Z_PlaneMeta add_tf);
		Z_PlaneMeta get_transform();
		void setTransform(Z_PlaneMeta new_tf);

		// Render execution
		void render(SDL_Renderer* renderer);
		void render(SDL_Renderer *renderer, Z_PlaneMeta transform);

		// Collision
		void setCollider(Z_PlaneMeta* box); // Coordinates are in sprite space
		Z_PlaneMeta* getCollider(); // Coordinates are in sprite space
		bool collision(Sprite* opponent); // Boolean collision
		Z_PlaneMeta getCollisionArea(Sprite* sprite); // Collide rect (intersection
};
