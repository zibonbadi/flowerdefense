#ifndef ANIMATION_H
#define ANIMATION_H

#include "headers.h"

class Animation {
	// Animation - An animated texture reference
	private:
		unsigned int last_update = 0; // Current timestamp
		unsigned int step_n = 0; // Reference to current frame
		unsigned int xsheet_i = 0; // x-sheet index
		unsigned int time_acc = 0; // Frame tick accumulation (correct misaligned drawcalls)
		float ticktime = 1000.0f/25; // Tick time (1000ms/Ticks per second)
		std::vector<SDL_Rect> frames;
		// Animation exposure sheet. 1st int = frame number; 2nd int = duration (in frameticks; <1 for infinite)
		std::vector<std::pair<int, int>> xsheet;
		// Base texture. Can be cropped into frames
		SDL_Texture* base;

		void print_map();
	public:
		Animation(SDL_Texture* base, unsigned int tickrate);
		~Animation();
		int advance(unsigned int now); // Advance animation (if duration > 0)
		int sync(unsigned int now); // Update timer without (prevent inset)
		int restart(); // Reset frame counters
		int add_frame(int u, int v, int uw, int vw);
		int add_frame(Z_PlaneMeta crop);
		int add_xsheet_phase(unsigned int frame, unsigned int duration);
		int clear_frames();
		int clear_xsheet();
		int clear_all();
		int set_rate(unsigned int tps);

		void render(SDL_Renderer* renderer);
		void render(SDL_Renderer* renderer, Z_PlaneMeta transform);
};
#endif


