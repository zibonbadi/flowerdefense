#pragma once
#include "headers.hh"

class Tilemap {
	// The tilemap consists of four parts
	// - An external base texture (from the TexturePool)
	// - A two-dimensional uint8_t array (a pointer) to store mappings
	// - An external collection of rects (SDL_Rect) to store tile associations
	// - A collection of Animation timers (in frame counts)
	// Tilemaps can be rendered on top of another, just like sprites
	private:
		//Tileset* tileset;
		// Map definition:
		// [SPACE] = hole (no tile; porous map)
		// "\n" = Line break
		//std::map<char, Tile*> tiles;
		std::map<char, Sprite*> tiles;
		//std::string map;
		std::map<std::pair<unsigned int,unsigned int>, char> map;
		Z_PlaneMeta transform_rect;

		/// Old
		//std::map<int, SDL_Rect> crops;
		//std::map<uint8_t, std::map<uint8_t, SDL_Rect*>> animations; // Animations (for later)
		// Pointer w/ width for unknown size
		int grit_w = 32, grit_h = 32;

		//std::map<std::pair<unsigned int,unsigned int>, int> tiles;

		void print_map();
	public:
		Tilemap();
		//Tilemap(SDL_Texture* base, int grit_w, int grit_h);
		Tilemap(int grit_w, int grit_h);
		~Tilemap();
		//int new_crop(int x, int y, int w, int h);
		//char new_crop(char id, Z_PlaneMeta crop);
		char add_tile(char id, Sprite* tile);
		int set_grit(int grit_w, int grit_h);
		void transform(Z_PlaneMeta transform);
		Z_PlaneMeta get_transform();
		void set_transform(Z_PlaneMeta new_tf);
		//int clear_crops();
		//int import_map(std::map<std::pair<unsigned int,unsigned int>, int> data);
		int import_map(std::map<std::pair<unsigned int,unsigned int>, char> data);
		void write(std::pair<unsigned int,unsigned int> top_left, std::string text);
		//std::map<std::pair<unsigned int,unsigned int>, int> get_map();
		std::map<std::pair<unsigned int,unsigned int>, char> get_map();
		//int get_spot(int x, int y);
		char get_spot(int x, int y);
		//int fill(int map_x, int map_y, int w, int h, int tile);
		int fill(int map_x, int map_y, int w, int h, char tile);
		void render(SDL_Renderer* renderer);
		void render(SDL_Renderer *renderer, Z_PlaneMeta transform);
};

