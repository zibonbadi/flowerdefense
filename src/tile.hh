#ifndef TILE_H
#define TILE_H

#include "headers.h"

class Tile {
	// Tile - Basically an animated texture reference
	private:
		unsigned int anime_n = 0;
		// Bounds spec:
		// MSB = Inner bounds
		// LSB = Outer bounds
		// See defs.h -> tilebound for flags
		uint8_t bounds_mask = 0b00000000;
		//std::map<uint8_t, std::map<uint8_t, SDL_Rect*>> animations; // Animations (for later)
		std::map<std::string, Animation*> anime; // Animations (for later)
		SDL_Texture* base;
	public:
		Tile();
		~Tile();
		int switch_to(int animation); // Switch to animation
		int set_bounds(uint8_t bounds);
		//int set_bounds(uint8_t bounds);
		int new_crop(int x, int y, int w, int h);
		int set_grit(int grit_w, int grit_h);
		int clear_crops();
		int import_map(std::map<std::pair<unsigned int,unsigned int>, int> data);
		std::map<std::pair<unsigned int,unsigned int>, int> get_map();
		int get_spot(int x, int y);
		int fill(int map_x, int map_y, int w, int h, int tile);
		void render(SDL_Renderer* renderer);
};
#endif


