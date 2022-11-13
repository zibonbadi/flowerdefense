#include "headers.hh"

int uninit(){
	IMG_Quit();
	SDL_Quit();
	return 0;
}

int poll(){
	SDL_Event e;
	while(SDL_PollEvent(&e)){
	switch(e.type){
	case SDL_QUIT:{
		throw std::runtime_error("Received QUIT signal");
		break;
	}
	case SDL_KEYDOWN:{
		switch(e.key.keysym.sym){
		case SDLK_q:{
			throw std::runtime_error("Received QUIT signal from user");
			break;
		}
		case SDLK_UP:
		case SDLK_w:{
			// Game logic
			//current_dir = UP;
			break;
		}
		case SDLK_DOWN:
		case SDLK_s:{
			// Game logic
			//current_dir = DOWN;
			break;
		}
		case SDLK_LEFT:
		case SDLK_a:{
			// Game logic
			//current_dir = LEFT;
			break;
		}
		case SDLK_RIGHT:
		case SDLK_d:{
			// Game logic
			//current_dir = RIGHT;
			break;
		}
		case SDLK_p:{
			//SDL_PauseAudio(adev, 1);
			break;
		}
		default:{
			break;
			}
		}
		break;
	}
	default:{
		break;
		}
	}
	}
	return 0;
};

int update(Tilemap* tmap){
	return 0;
};

int main(int argc, char* argv[]){
	std::cout << "Engine launched!" << std::endl;

	try{
		std::cout << "Building system..." << std::endl;
		Z_RGBA bgcolor = { .r = 0x00, .g = 0x00, .b = 0x00 };
		Game screen((int)800,(int)800, bgcolor);


			ResourceManager rc(screen.getRenderer());
			// rc.import_texture("grass", "./assets/grass_2.jpeg");
			// rc.import_texture("rose", "./assets/rose.png");

			// Tilemap ground(32,32), plants(160,160);
			// auto grass = rc.make_static_sprite_from_texture("tiles.grass", "grass", Z_PlaneMeta {.u = 0, .v = 0, .uw = 32, .vw = 32}).second;
			// auto rose = rc.make_static_sprite_from_texture("tiles.flowers.rose", "banana", Z_PlaneMeta {.u = 0, .v = 0, .uw = 32, .vw = 32}).second;
			// rose->set_color(Z_RGBA{ .r = 0x22, .g = 0xaa, .b = 0xff, .a = 0xff});
			// ground.add_tile('.',grass);
			// plants.add_tile('%',rose);
			rc.import_texture("spritesheet", "./assets/Flowerdefense_Spritesheet.png");
			rc.import_texture("dirt", "./assets/dirt.png");
			rc.import_texture("rose", "./assets/rose.png");

			auto grass = rc.make_static_sprite_from_texture("grass", "spritesheet", Z_PlaneMeta{ .u = 0, .v = 32 * 7, .uw = 32, .vw = 32 }).second;
			
			auto playerBottom = Animation(rc.get_texture("spritesheet"), 5);

			playerBottom.add_frame(Z_PlaneMeta{ .u = 0, .v = 32 * 2, .uw = 32, .vw = 32 });
			playerBottom.add_frame(Z_PlaneMeta{ .u = 32 * 1, .v = 32 * 2, .uw = 32, .vw = 32 });
			playerBottom.add_frame(Z_PlaneMeta{ .u = 32 * 2, .v = 32 * 2, .uw = 32, .vw = 32 });
			playerBottom.add_frame(Z_PlaneMeta{ .u = 32 * 3, .v = 32 * 2, .uw = 32, .vw = 32 });
			playerBottom.add_frame(Z_PlaneMeta{ .u = 32 * 4, .v = 32 * 2, .uw = 32, .vw = 32 });

			playerBottom.add_xsheet_phase(0, 1);
			playerBottom.add_xsheet_phase(1, 1);
			playerBottom.add_xsheet_phase(2, 1);
			playerBottom.add_xsheet_phase(3, 1);
			playerBottom.add_xsheet_phase(4, 1);
			auto player = Sprite("down", &playerBottom, Z_PlaneMeta{ .x = 400-64, .y = 200, .w = 128, .h = 128, });

			Tilemap ground(32,32), plants(32,32);
			auto dirt = rc.make_static_sprite_from_texture("tiles.dirt", "dirt", Z_PlaneMeta {.u = 0, .v = 0, .uw = 85, .vw = 85}).second;
			auto rose = rc.make_static_sprite_from_texture("tiles.flowers.rose", "rose", Z_PlaneMeta {.u = 0, .v = 0, .uw = 32, .vw = 32}).second;
			ground.add_tile('.', grass);
			plants.add_tile('%', rose);

			ground.fill(0, 0, 25, 25, '.');
			plants.fill(12, 12, 1, 1, '%');

			/* Actual game */
			Plane board(Z_PlaneMeta{.x = 0, .y = 0, .w = 800, .h = 800});
			
			// Construct scene planes
			board.attach(&ground);
			board.attach(&plants);
			board.attach(&player);

			// Hook plane into scene
			screen.attach(&board);

			std::cout << "Entering main loop..." << std::endl;

			// Gloptastic tracker beats
			if(rc.import_mod("bgm", "./assets/cycle.stm") != nullptr){
				screen.load_mod(rc.get_mod("bgm"), -1, -1);
			}

			auto now = SDL_GetTicks();
			uint32_t past = 0;

			while(true){
				poll();
				playerBottom.advance(now);
				screen.render();
				past = now;
			}
		}catch(std::exception& e){
			std::cerr << "Exception caught: " << e.what() << std::endl;
		}
		std::cout << "Main loop stopped." << std::endl;
	std::cout << "Shutting down..." << std::endl;
	uninit();
	std::cout << "Engine stopped!" << std::endl;
	return 0;
}
