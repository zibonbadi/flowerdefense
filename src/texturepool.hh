#ifndef TEXTUREPOOL_H
#define TEXTUREPOOL_H

#include "headers.h"

class TexturePool {
	// Store textures in graphics memory and easily manage them
	private:
		std::map<std::string, SDL_Texture*> textures;
		SDL_Renderer* renderer;
	public:
		TexturePool(SDL_Renderer* renderer);
		~TexturePool();
		SDL_Texture* import(std::string id, std::string path);
		int erase(std::string id);
		SDL_Texture* get(std::string id);
};
#endif

