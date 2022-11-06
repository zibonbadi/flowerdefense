#include "texturepool.hh"

TexturePool::TexturePool(SDL_Renderer* renderer){
	this->renderer = renderer;
};

TexturePool::~TexturePool(){
};

SDL_Texture* TexturePool::import(std::string id, std::string path){
	SDL_Texture* importbuf = IMG_LoadTexture(this->renderer, path.c_str());
	this->textures.insert(std::pair<std::string,SDL_Texture*>(id,importbuf));
	if(importbuf){
		std::clog << "Added '" << id << "':'" << path << "' to TexturePool " << this << std::endl;
		return importbuf;
	}
	std::cerr << "TexturePool.import() error: " << IMG_GetError() << std::endl;
	return nullptr;
};

int TexturePool::erase(std::string id){
	try{
		auto it = this->textures.find(id);
		if (it != this->textures.end()){
			SDL_DestroyTexture(this->textures[id]);
			this->textures.erase(it);
		};
		return 0;
	}catch(std::exception& e){
		std::cerr << "TexturePool.erase() error: " << e.what() << std::endl;
		return -1;
	}
};

SDL_Texture* TexturePool::get(std::string id){
	if(this->textures[id]){
		return this->textures[id];
	}
	return nullptr;
};

