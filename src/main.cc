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
		constexpr int SCREEN_HEIGHT = 800;
		constexpr int SCREEN_WIDTH = 800;
		Game screen((int)SCREEN_WIDTH,(int)SCREEN_HEIGHT, bgcolor);

			EventBus ebus;
			EBus_Fn quit_func = [&](Event* e){
				if(e->get("status_edge") == "up"){
					std::clog << "Hello from event Callback: " << e << std::endl;
					running = false;
				};
			};
			ebus.subscribe("engine.quit", &quit_func);

			KeyMapper keymap(&ebus);
			Event e_quit("engine.quit");
			e_quit.set("attrib", "banana");
			keymap.bind(SDLK_q, e_quit);

			ResourceManager rc(screen.getRenderer());
			/* Import spritesheet */
			rc.import_texture("spritesheet", "./assets/spritesheet.png");

			/* Select tiles from tileset */
			auto grass = rc.make_static_sprite_from_texture("tiles.grass", "spritesheet", Z_PlaneMeta{ .u = 32 * 4, .v = 32 * 5, .uw = 32, .vw = 32 }).second;
			auto rose = rc.make_static_sprite_from_texture("tiles.rose", "spritesheet", Z_PlaneMeta{ .u = 32 * 5, .v = 32 * 5, .uw = 32, .vw = 32 }).second;

			/* Player animation definition */
			auto playerLeft = Animation(rc.get_texture("spritesheet"), 5);

			playerLeft.add_frame(Z_PlaneMeta{ .u = 0, .v = 32 * 2, .uw = 32, .vw = 32 });
			playerLeft.add_frame(Z_PlaneMeta{ .u = 32 * 1, .v = 32 * 2, .uw = 32, .vw = 32 });
			playerLeft.add_frame(Z_PlaneMeta{ .u = 32 * 2, .v = 32 * 2, .uw = 32, .vw = 32 });
			playerLeft.add_frame(Z_PlaneMeta{ .u = 32 * 3, .v = 32 * 2, .uw = 32, .vw = 32 });
			playerLeft.add_frame(Z_PlaneMeta{ .u = 32 * 4, .v = 32 * 2, .uw = 32, .vw = 32 });

			playerLeft.add_xsheet_phase(0, 1);
			playerLeft.add_xsheet_phase(1, 1);
			playerLeft.add_xsheet_phase(2, 1);
			playerLeft.add_xsheet_phase(3, 1);
			playerLeft.add_xsheet_phase(4, 1);
			playerLeft.add_xsheet_phase(3, 1);
			playerLeft.add_xsheet_phase(2, 1);
			playerLeft.add_xsheet_phase(1, 1);
			rc.add_anim("player.left", &playerLeft);

			auto playerRight = Animation(rc.get_texture("spritesheet"), 5);

			playerRight.add_frame(Z_PlaneMeta{ .u = 0, .v = 32 * 3, .uw = 32, .vw = 32 });
			playerRight.add_frame(Z_PlaneMeta{ .u = 32 * 1, .v = 32 * 3, .uw = 32, .vw = 32 });
			playerRight.add_frame(Z_PlaneMeta{ .u = 32 * 2, .v = 32 * 3, .uw = 32, .vw = 32 });
			playerRight.add_frame(Z_PlaneMeta{ .u = 32 * 3, .v = 32 * 3, .uw = 32, .vw = 32 });
			playerRight.add_frame(Z_PlaneMeta{ .u = 32 * 4, .v = 32 * 3, .uw = 32, .vw = 32 });

			playerRight.add_xsheet_phase(0, 1);
			playerRight.add_xsheet_phase(1, 1);
			playerRight.add_xsheet_phase(2, 1);
			playerRight.add_xsheet_phase(3, 1);
			playerRight.add_xsheet_phase(4, 1);
			playerRight.add_xsheet_phase(3, 1);
			playerRight.add_xsheet_phase(2, 1);
			playerRight.add_xsheet_phase(1, 1);

			rc.add_anim("player.right", &playerRight);



			//auto player = rc.make_sprite_from_anim("player", "player.left", Z_PlaneMeta { .x = 0, .y = 0, .w = 64, .h = 64 }).second;
			auto player = rc.make_sprite("player", Z_PlaneMeta { .x = 0, .y = 0, .w = 64, .h = 64 }).second;
			player->add_animation("left", rc.get_anim("player.left"));
			player->switch_to_anim("left");
			/*auto playerSprite = rc.get_sprite("player").second;
			rc.add_anim("player.right", &playerRight);
			playerSprite.add_animation("up", rc.get_anim("player.up"));
			playerSprite.switch_to_anim("player.up");*/

			Tilemap ground(32,32), plants(32,32);

			
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

			/* Player data definition */
			float playerSpeed = 4.f;
			SDL_FPoint playerCoordinates = { .x = (SCREEN_WIDTH / 2) - 64, .y = (SCREEN_HEIGHT / 2) - 64 - 200 };
			SDL_FPoint delta = { .x = 0, .y = 0 };
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
				case SDL_KEYUP:
				case SDL_KEYDOWN:{
					keymap.probe(e.key);
				}
				default:{
					break;
					}
				}
				}
				//playerBottom.advance(now);
				const Uint8* state = SDL_GetKeyboardState(nullptr);


				/* Update player */
				delta.x = 0;
				delta.y = 0;
				if (state[SDL_SCANCODE_UP])		delta.y = -1;
				if (state[SDL_SCANCODE_DOWN])	delta.y = 1;
				if (state[SDL_SCANCODE_LEFT])	delta.x = -1;
				if (state[SDL_SCANCODE_RIGHT])	delta.x = 1;

				/* Normalize delta length */
				if (delta.x != 0 || delta.y != 0)
				{
					const float length_inverse = 1.f / sqrt(delta.x * delta.x + delta.y * delta.y);
					delta.x *= length_inverse * playerSpeed;
					delta.y *= length_inverse * playerSpeed;
				}

				/* Add delta to player coordinates */
				playerCoordinates.x += delta.x;
				playerCoordinates.y += delta.y;

				/* Handle Offscreen Movement */
				if (playerCoordinates.y < 0)				playerCoordinates.y += delta.y;
				if (playerCoordinates.y > SCREEN_HEIGHT)	playerCoordinates.y -= delta.y;
				if (playerCoordinates.x < 0)				playerCoordinates.x += delta.x;
				if (playerCoordinates.x > SCREEN_WIDTH)		playerCoordinates.x -= delta.x;

				/* Adjust player sprite position*/
				player->setTransform(Z_PlaneMeta{ .x = playerCoordinates.x, .y = playerCoordinates.y, .w = 128, .h = 128 });

				/* Advance the player animation */
				//playerLeft.advance(now);
				rc.advance_all_anim(now);
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
