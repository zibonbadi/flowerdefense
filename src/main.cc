#include "headers.hh"

int uninit(){
	IMG_Quit();
	SDL_Quit();
	return 0;
}

bool running = true;

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

			EventBus ebus;
			EBus_Fn quit_func = [](Event* e){ running = false; };
			ebus.subscribe("engine.quit", &quit_func);

			KeyMapper keymap(&ebus);
			Event e_quit("engine.quit");
			e_quit.set("attrib", "banana");
			keymap.bind(SDLK_q, e_quit);

			ResourceManager rc(screen.getRenderer());
			rc.import_texture("spritesheet", "./assets/Flowerdefense_Spritesheet.png");

			
			auto playerBottom = Animation(rc.get_texture("spritesheet"), 5);

			playerBottom.add_frame(Z_PlaneMeta{ .u = 32 * 0, .v = 32 * 1, .uw = 32, .vw = 32 });
			playerBottom.add_frame(Z_PlaneMeta{ .u = 32 * 1, .v = 32 * 1, .uw = 32, .vw = 32 });
			playerBottom.add_frame(Z_PlaneMeta{ .u = 32 * 2, .v = 32 * 1, .uw = 32, .vw = 32 });
			playerBottom.add_frame(Z_PlaneMeta{ .u = 32 * 3, .v = 32 * 1, .uw = 32, .vw = 32 });
			playerBottom.add_frame(Z_PlaneMeta{ .u = 32 * 4, .v = 32 * 1, .uw = 32, .vw = 32 });

			playerBottom.add_xsheet_phase(0, 1);
			playerBottom.add_xsheet_phase(1, 1);
			playerBottom.add_xsheet_phase(2, 1);
			playerBottom.add_xsheet_phase(3, 1);
			playerBottom.add_xsheet_phase(4, 1);
			rc.add_anim("player.down", &playerBottom);
			auto player = rc.make_sprite_from_anim("player", "player.down", Z_PlaneMeta { .x = 400-64, .y = 200, .w = 128, .h = 128 }).second;

			Tilemap ground(32,32), plants(32,32);
			auto dirt = rc.make_static_sprite_from_texture("tiles.dirt", "spritesheet", Z_PlaneMeta {.u = 0, .v = 0, .uw = 85, .vw = 85}).second;
			auto grass = rc.make_static_sprite_from_texture("grass", "spritesheet", Z_PlaneMeta{ .u = 32 * 4, .v = 32 * 5, .uw = 32, .vw = 32 }).second;
			auto rose = rc.make_static_sprite_from_texture("tiles.flowers.rose", "banana", Z_PlaneMeta {.u = 32*5, .v = 32*5, .uw = 32, .vw = 32}).second;
			
			// Set backup color for rose tile
			rose->set_color(Z_RGBA{ .r = 0xff, .g = 0x7f, .b = 0xcf, .a = 0xff});

			ground.add_tile('.', grass);
			plants.add_tile('%', rose);

			ground.fill(0, 0, 25, 25, '.');
			plants.fill(12, 12, 1, 1, '%');

			/* Actual game */
			Plane board(Z_PlaneMeta{.x = 0, .y = 0, .w = 800, .h = 800});
			
			// Construct scene planes
			board.attach(&ground);
			board.attach(&plants);
			board.attach(player);

			// Hook plane into scene
			screen.attach(&board);

			std::cout << "Entering main loop..." << std::endl;

			// Gloptastic tracker beats
			if(rc.import_mod("bgm", "./assets/cycle.stm") != nullptr){
				screen.load_mod(rc.get_mod("bgm"), -1, -1);
			}

			uint32_t past = 0;
			while(running){
				auto now = SDL_GetTicks();
				//poll();
				SDL_Event e;
				while(SDL_PollEvent(&e)){
				switch(e.type){
				case SDL_QUIT:{
					throw std::runtime_error("Received QUIT signal");
					break;
				}
				case SDL_KEYDOWN:{
					keymap.probe(e.key);
				}
				default:{
					break;
					}
				}
				}
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
