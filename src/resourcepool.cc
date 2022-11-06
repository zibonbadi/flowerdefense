#include "resourcepool.hh"

/* Basics */
ResourcePool::ResourcePool(SDL_Renderer* renderer){
	this->renderer = renderer;
};

ResourcePool::~ResourcePool(){
};

static std::string make_unique_string(){
	return "";
}


/* Textures */
std::pair<std::string, SDL_Texture*> ResourcePool::import_texture(std::string id, std::string path){
	SDL_Texture* importbuf = IMG_LoadTexture(this->renderer, path.c_str());
	if(importbuf){
		return this->import_texture(id, importbuf);
	}
	std::cerr << "ResourcePool.import_texture(\"" << id << "\",\"" << path << "\") error: " << IMG_GetError() << std::endl;
	//return nullptr;
	return std::pair(id,nullptr);
};

std::pair<std::string, SDL_Texture*> ResourcePool::import_texture(std::string id, SDL_Texture* tex){
	if(tex){
		this->textures.insert(std::pair<std::string,SDL_Texture*>(id,tex));
		std::clog << "Added texture '" << id << "':(" << tex << ") to ResourcePool " << this << std::endl;
		//return importbuf;
		return std::pair(id,tex);
	}
	std::cerr << "ResourcePool.import_texture(\"" << id << "\",<SDL_Texture*> " << tex << ") error: " << IMG_GetError() << std::endl;
	//return nullptr;
	return std::pair(id,nullptr);
};

int ResourcePool::free_texture(std::string id){
	try{
		auto it = this->textures.find(id);
		if (it != this->textures.end()){
			std::clog << "Freed texture'" << id << "':(" << this->textures[id] << ") from ResourcePool " << this << std::endl;
			SDL_DestroyTexture(this->textures[id]);
			this->textures.erase(it);
		};
		return 0;
	}catch(std::exception& e){
		std::cerr << "ResourcePool.free_texture() error: " << e.what() << std::endl;
		return -1;
	}
};

SDL_Texture* ResourcePool::get_texture(std::string id){
	if(this->textures[id]){
		return this->textures[id];
	}
	return nullptr;
};

/* Modules */
openmpt::module* ResourcePool::import_mod(std::string id, std::string path){
	try{
		//if(this->mod_file != nullptr){ this->mod_file = nullptr; }
		std::ifstream modfile(path, std::ios::binary);
		if(!modfile.good()){ throw std::runtime_error("Unable to open file"); }
		
		// Avoid memory leakage
		if (this->mods.find(id) != this->mods.end()){ delete this->mods[id]; }
		this->mods[id] = new openmpt::module(modfile);
		std::clog << "Added module '" << id << "':(" << this->mods[id] << ") to ResourcePool " << this << std::endl;
		return 0;
	}catch(std::exception &e){
		std::cerr << "ResourcePool.import_mod(" << path << ") exception: " << e.what() << std::endl;
		return nullptr;
	}
}

int ResourcePool::free_mod(std::string id){
	try{
		auto it = this->mods.find(id);
		if (it != this->mods.end()){
			delete this->mods[id];
			this->mods.erase(it);
		};
		return 0;
	}catch(std::exception& e){
		std::cerr << "ResourcePool.free_mod() error: " << e.what() << std::endl;
		return -1;
	}
};


openmpt::module* ResourcePool::get_mod(std::string id){
	if(this->mods[id]){
		return this->mods[id];
	}
	return nullptr;
};

/* Animation */
std::pair<std::string, Animation*> ResourcePool::add_anim(std::string id, Animation* anim){
	if(anim){
		std::clog << "Added Animation '" << id << "':(<Animation*> " << anim << ") to ResourcePool " << this << std::endl;
		this->animations.insert(std::pair(id,anim));
		return std::pair(id,anim);
	}
	std::cerr << "ResourcePool.add_anim(\"" << id << "\",<Animation*>" << anim << ") error: " << IMG_GetError() << std::endl;
	//return nullptr;
	return std::pair(id,nullptr);
};
Animation* ResourcePool::get_anim(std::string id){
	if(this->animations[id]){
		return animations[id];
	}
	return nullptr;
};
int ResourcePool::free_anim(std::string id){
	try{
		auto it = this->animations.find(id);
		if (it != this->animations.end()){
			delete this->animations[id];
			this->animations.erase(it);
		};
		return 0;
	}catch(std::exception& e){
		std::cerr << "ResourcePool.free_anim() error: " << e.what() << std::endl;
		return -1;
	}
};


std::pair<std::string, Animation*> ResourcePool::make_static_anim_from_file(std::string id, std::string path, Z_PlaneMeta crop){
	auto new_texture = this->import_texture(id+".texture.default", path);
	return this->make_static_anim_from_texture(id, this->get_texture(new_texture.first), crop);
}
std::pair<std::string, Animation*> ResourcePool::make_static_anim_from_texture(std::string id, SDL_Texture* tex, Z_PlaneMeta crop){
	Animation *new_anim = new Animation(tex, 1);
	new_anim->add_frame(crop.u, crop.v, crop.uw, crop.vw);
	new_anim->add_xsheet_phase(0,0);

	if (this->animations.find(id) != this->animations.end()){ delete this->animations[id]; }
	this->animations[id] = new_anim;

	std::clog << "Added animation '" << id << "':(" << new_anim << ") to ResourcePool " << this << std::endl;

	//return this->animations.find(id)->second;
	return std::pair(id,new_anim);
}
std::pair<std::string, Animation*> ResourcePool::make_static_anim_from_texture(std::string id, std::string texId, Z_PlaneMeta crop){
	return this->make_static_anim_from_texture(id, this->get_texture(texId), crop);
}


/* Sprites */
std::pair<std::string, Sprite*> ResourcePool::add_sprite(std::string id, Sprite* spr){
	if(spr){
		std::clog << "Added sprite '" << id << "':(<Sprite*> " << spr << ") to ResourcePool " << this << std::endl;
		this->sprites.insert(std::pair(id,spr));
		return std::pair(id,spr);
	}
	std::cerr << "ResourcePool.add_sprite(\"" << id << "\",<Sprite*>" << spr << ") error: " << IMG_GetError() << std::endl;
	//return nullptr;
	return std::pair(id,nullptr);
};
Sprite* ResourcePool::get_sprite(std::string id){
	if(this->sprites[id]){
		return sprites[id];
	}
	return nullptr;
};
int ResourcePool::free_sprite(std::string id){
	try{
		auto it = this->sprites.find(id);
		if (it != this->sprites.end()){
			delete this->sprites[id];
			this->sprites.erase(it);
		};
		return 0;
	}catch(std::exception& e){
		std::cerr << "ResourcePool.free_sprite() error: " << e.what() << std::endl;
		return -1;
	}
};

std::pair<std::string, Sprite*> ResourcePool::make_static_sprite_from_anim(std::string id, Animation* anim, Z_PlaneMeta crop){
	Sprite* new_sprite = new Sprite("show", anim, crop );
	this->sprites.insert(std::pair(id,new_sprite));
	std::clog << "Added sprite '" << id << "':(" << new_sprite << ") to ResourcePool " << this << std::endl;
	return std::pair(id,new_sprite);
}
std::pair<std::string, Sprite*> ResourcePool::make_static_sprite_from_anim(std::string id, std::string animId, Z_PlaneMeta crop){
	return this->make_static_sprite_from_anim(id, this->get_anim(animId), crop);
}
std::pair<std::string, Sprite*> ResourcePool::make_static_sprite_from_texture(std::string id, SDL_Texture* tex, Z_PlaneMeta crop){
	auto new_anim = this->make_static_anim_from_texture(id+".animation", tex, crop);
	return this->make_static_sprite_from_anim(id, new_anim.second, crop);
}
std::pair<std::string, Sprite*> ResourcePool::make_static_sprite_from_texture(std::string id, std::string texId, Z_PlaneMeta crop){
	return this->make_static_sprite_from_texture(id, this->get_texture(texId), crop);
}
std::pair<std::string, Sprite*> ResourcePool::make_static_sprite_from_file(std::string id, std::string path, Z_PlaneMeta crop){
	auto new_anim = this->make_static_anim_from_file(id+".animation", path, crop);
	return this->make_static_sprite_from_anim(id, new_anim.second, crop);
}

