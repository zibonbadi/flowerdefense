#pragma once
#include "headers.hh"
#include "sprite.hh"
#include "tilemap.hh"

class Plane {
	// Visual plane for internal graphics model
	// 3 layer scene graph: Canvas -> Plane (-> Tilemap) -> Sprite
	
	private:
		// Let the background
		//uint8_t flags[] = 0b000000000000;
		uint8_t flags = 0b000000000000;
		float zfactor = 1; // Parallax z factor
		bool visible = true; // Visible on-demand (runtime)
		//bool locked_transform = false; // Prevent transformations (screen buffers)
		Z_PlaneMeta transform_rect;

		// Ordered map for render order
		//std::map<int, Tilemap*> tilemaps;
		//std::map<int, Sprite*> sprites;
		std::vector<Tilemap*> tilemaps;
		std::vector<Sprite*> sprites;
		
	public:
		Plane();
		Plane(Z_PlaneMeta init);
		~Plane();
		void attach(Tilemap* src);
		void attach(Sprite* src);
		void detach(Tilemap* src);
		void detach(Sprite* src);
		void set_visible(bool value);
		bool is_visible();
		void toggle_visible();
		void transform(Z_PlaneMeta transform);
		Z_PlaneMeta get_transform();
		void set_transform(Z_PlaneMeta new_tf);
		void render(SDL_Renderer *renderer);
};
