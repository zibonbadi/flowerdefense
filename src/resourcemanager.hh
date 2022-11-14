#ifndef RESOURCEPOOL_H
#define RESOURCEPOOL_H

#include "headers.hh"

class ResourceManager {
	// Store textures in graphics memory and easily manage them
	private:
		/* Setup */
		SDL_Renderer* renderer;
		
		/* Containers */
		std::map<std::string, SDL_Texture*> textures;
		std::map<std::string, openmpt::module*> mods;
		std::map<std::string, Animation*> animations;
		std::map<std::string, Sprite*> sprites;
	public:
		ResourceManager(SDL_Renderer* renderer);
		~ResourceManager();

		/* Textures */
		//SDL_Texture* import_texture(std::string id, std::string path);
		std::pair<std::string, SDL_Texture*> import_texture(std::string id, std::string path);
		std::pair<std::string, SDL_Texture*> import_texture(std::string id, SDL_Texture* tex);
		int free_texture(std::string id);
		SDL_Texture* get_texture(std::string id);

		/* Modules */
		openmpt::module* import_mod(std::string id, std::string path);
		int free_mod(std::string id);
		openmpt::module* get_mod(std::string id);


		/* Animations */
		std::pair<std::string, Animation*> add_anim(std::string id, Animation* spr);
		int free_anim(std::string id);
		Animation* get_anim(std::string id);

		std::pair<std::string, Animation*> make_static_anim_from_file(std::string id, std::string path, Z_PlaneMeta crop);
		std::pair<std::string, Animation*> make_static_anim_from_texture(std::string id, SDL_Texture* tex, Z_PlaneMeta crop);
		std::pair<std::string, Animation*> make_static_anim_from_texture(std::string id, std::string texId, Z_PlaneMeta crop);
		void advance_all_anim(uint32_t now);


		/* Sprites */
		std::pair<std::string, Sprite*> add_sprite(std::string id, Sprite* spr);
		int free_sprite(std::string id);
		Sprite* get_sprite(std::string id);

		std::pair<std::string, Sprite*> make_sprite(std::string id, Z_PlaneMeta crop);
		std::pair<std::string, Sprite*> make_sprite_from_anim(std::string id, Animation* anim, Z_PlaneMeta crop);
		std::pair<std::string, Sprite*> make_sprite_from_anim(std::string id, std::string animId, Z_PlaneMeta crop);
		std::pair<std::string, Sprite*> make_static_sprite_from_texture(std::string id, SDL_Texture* tex, Z_PlaneMeta crop);
		std::pair<std::string, Sprite*> make_static_sprite_from_texture(std::string id, std::string texId, Z_PlaneMeta crop);
		std::pair<std::string, Sprite*> make_static_sprite_from_file(std::string id, std::string path, Z_PlaneMeta crop);

		/* Tilemaps */
};

//ResourceManager g_resourcemanager;

#endif

