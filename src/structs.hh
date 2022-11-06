#ifndef STRUCTS_H
#define STRUCTS_H
/*
typedef struct {
	SDL_Renderer *renderer;
	SDL_Window *window;
	SDL_Texture *framebuf;
} Z_Video;
*/

typedef struct Z_Cel {
	// Header data. width, height & bytes per pixel
	uint8_t w,h,bpp;
	char* data;
	Z_Cel(){
		data = (char*) calloc(w*h, bpp);
	};

} Z_Cel;

typedef struct Z_RGBA {
	uint8_t r = 0xff,
		g = 0xff,
		b = 0xff,
		a = 0xff;
} Z_RGBA;

typedef struct Z_Time {
	uint32_t ms;
} Z_Time;

typedef struct Z_PlaneMeta {
	float 
		x = 0.0f,
		y = 0.0f,
		w = 0.0f,
		h = 0.0f,
		// For transformations (dest pixels)
		pivot_x = 0.0f,
		pivot_y = 0.0f,
		deg = 0.0f,
		// Texture crop
		u = 0.0f,
		v = 0.0f,
		uw = 0.0f,
		vw = 0.0f
	;
} Z_PlaneMeta;
#endif
