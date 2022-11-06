#ifndef CANVAS_H
#define CANVAS_H

#include "headers.hh"

class Canvas {
	// Generic canvas; basically a video output
	private:
		uint16_t w,h;
		SDL_Renderer *renderer;
		SDL_Window *window;
		SDL_Texture *framebuf;
		Z_RGBA background;
		// 3/4 layer scene graph: Canvas -> Plane (-> Tilemap) -> Sprite
		std::vector<Plane*> planes;

		/* Audio stuff */
		SDL_AudioDeviceID a_master;
		SDL_AudioSpec* a_specs;
		std::vector<float> a_buf_interleaved;
		//std::ifstream *mod_file = nullptr;
		openmpt::module *mod_buf = nullptr;

		void _init(uint16_t w, uint16_t h);

	public:
		Canvas();
		Canvas(Z_RGBA background);
		Canvas(uint16_t w, uint16_t h, Z_RGBA background);
		Canvas(uint16_t w, uint16_t h);
		~Canvas();
		SDL_Renderer* getRenderer();
		SDL_Window* getWindow();
		SDL_Texture* getFramebuffer();
		void blit(Plane *src);
		void set_background(Z_RGBA);
		void attach(Plane *src);
		void render();
		/* Audio */
		int load_mod(std::string path, int32_t subsong = -1, int32_t repeats = 0);
		int load_mod(openmpt::module* mod, int32_t subsong = -1, int32_t repeats = 0);
};
#endif

