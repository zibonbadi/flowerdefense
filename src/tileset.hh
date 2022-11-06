#ifndef TILESET_H
#define TILESET_H

#include "headers.hh"

class Tileset {
	// Tileset - Literally just a collection of Tiles
	private:
		std::map<char, Tile> tiles;
		std::map<uint8_t, std::map<uint8_t, SDL_Rect*>> animations; // Animations (for later)

		void print_map();
	public:
		Tileset();
		~Tileset();
		int add_tile();
		int set_grit(int grit_w, int grit_h);
		int clear_crops();
		int import_map(std::map<std::pair<unsigned int,unsigned int>, int> data);
		std::map<std::pair<unsigned int,unsigned int>, int> get_map();
		int get_spot(int x, int y);
		int fill(int map_x, int map_y, int w, int h, int tile);
		void render(SDL_Renderer* renderer);
};
#endif


