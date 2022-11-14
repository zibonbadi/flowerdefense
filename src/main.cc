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

typedef enum
{
	LEFT,
	RIGHT,
	UP,
	DOWN
} EPlayerDirection;

int main(int argc, char* argv[]){
	std::cout << "Engine launched!" << std::endl;

	try{
		std::cout << "Building system..." << std::endl;
		Z_RGBA bgcolor = { .r = 0x00, .g = 0x00, .b = 0x00 };
		constexpr int SCREEN_HEIGHT = 800;
		constexpr int SCREEN_WIDTH = 800;
		Game screen((int)SCREEN_WIDTH,(int)SCREEN_HEIGHT, bgcolor);

			EventBus ebus;
			KeyMapper keymap(&ebus);

			ResourceManager rc(screen.getRenderer());
			/* Import spritesheet */
			rc.import_texture("spritesheet", "./assets/spritesheet.png");

			/* Select tiles from tileset */
			auto grass = rc.make_static_sprite_from_texture("tiles.grass", "spritesheet", Z_PlaneMeta{ .u = 32 * 4, .v = 32 * 5, .uw = 32, .vw = 32 }).second;
			auto rose = rc.make_static_sprite_from_texture("tiles.rose", "spritesheet", Z_PlaneMeta{ .u = 32 * 5, .v = 32 * 5, .uw = 32, .vw = 32 }).second;

			/* Player animation definition */

			auto playerUp = Animation(rc.get_texture("spritesheet"), 5);
			playerUp.add_frame(Z_PlaneMeta{ .u = 0, .v = 32 * 1, .uw = 32, .vw = 32 });
			playerUp.add_frame(Z_PlaneMeta{ .u = 32 * 1, .v = 32 * 1, .uw = 32, .vw = 32 });
			playerUp.add_frame(Z_PlaneMeta{ .u = 32 * 2, .v = 32 * 1, .uw = 32, .vw = 32 });
			playerUp.add_frame(Z_PlaneMeta{ .u = 32 * 3, .v = 32 * 1, .uw = 32, .vw = 32 });
			playerUp.add_frame(Z_PlaneMeta{ .u = 32 * 4, .v = 32 * 1, .uw = 32, .vw = 32 });
			playerUp.add_xsheet_phase(0, 1);
			playerUp.add_xsheet_phase(1, 1);
			playerUp.add_xsheet_phase(2, 1);
			playerUp.add_xsheet_phase(3, 1);
			playerUp.add_xsheet_phase(4, 1);
			playerUp.add_xsheet_phase(3, 1);
			playerUp.add_xsheet_phase(2, 1);
			playerUp.add_xsheet_phase(1, 1);
			rc.add_anim("player.up", &playerUp);

			auto playerDown = Animation(rc.get_texture("spritesheet"), 5);
			playerDown.add_frame(Z_PlaneMeta{ .u = 0, .v = 32 * 0, .uw = 32, .vw = 32 });
			playerDown.add_frame(Z_PlaneMeta{ .u = 32 * 1, .v = 32 * 0, .uw = 32, .vw = 32 });
			playerDown.add_frame(Z_PlaneMeta{ .u = 32 * 2, .v = 32 * 0, .uw = 32, .vw = 32 });
			playerDown.add_frame(Z_PlaneMeta{ .u = 32 * 3, .v = 32 * 0, .uw = 32, .vw = 32 });
			playerDown.add_frame(Z_PlaneMeta{ .u = 32 * 4, .v = 32 * 0, .uw = 32, .vw = 32 });
			playerDown.add_xsheet_phase(0, 1);
			playerDown.add_xsheet_phase(1, 1);
			playerDown.add_xsheet_phase(2, 1);
			playerDown.add_xsheet_phase(3, 1);
			playerDown.add_xsheet_phase(4, 1);
			playerDown.add_xsheet_phase(3, 1);
			playerDown.add_xsheet_phase(2, 1);
			playerDown.add_xsheet_phase(1, 1);
			rc.add_anim("player.down", &playerDown);

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

			Sprite player;
			rc.add_sprite("player", &player);
			player.add_animation("up", rc.get_anim("player.up"));
			player.add_animation("down", rc.get_anim("player.down"));
			player.add_animation("left", rc.get_anim("player.left"));
			player.add_animation("right", rc.get_anim("player.right"));
			player.switch_to_anim("left");

			EPlayerDirection playerDir = EPlayerDirection::LEFT;

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
			board.attach(&player);

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
			SDL_FPoint playerCoordinates = { .x = (SCREEN_WIDTH / 2) - 32, .y = (SCREEN_HEIGHT / 2) - 32 - 200 };
			SDL_FPoint delta = { .x = 0, .y = 0 };
			EPlayerDirection pastPlayerDir;

			/* Set up control events */
			EBus_Fn f_quit = [&](Event* e){
				if(e->get("status_edge") == "up"){
					std::clog << "Quitting engine on behalf of event " << e << std::endl;
					running = false;
				};
			};
			EBus_Fn f_set_dir = [&](Event* e){
				auto dir = e->get("direction");
				if(e->get("status_edge") == "down"){
					if(dir == "right"){
						delta.x = 1;
						playerDir = EPlayerDirection::RIGHT; 
					};
					if(dir == "up"){
						delta.y = -1;
						playerDir = EPlayerDirection::UP; 
					};
					if(dir == "left"){
						delta.x = -1;
						playerDir = EPlayerDirection::LEFT; 
					};
					if(dir == "down"){
						delta.y = 1;
						playerDir = EPlayerDirection::DOWN; 
					};
				};
				if(e->get("status_edge") == "up"){
					if(dir == "left" || dir == "right"){
						delta.x = 0;
					}
					if(dir == "up" || dir == "down"){
						delta.y = 0;
					}
				}
			};

			// Create Keymap events
			Event e_quit("engine.quit");
			keymap.bind(SDLK_q, e_quit);
			// Up
			Event e_player_up("player.set_direction");
			e_player_up.set("direction", "up");
			keymap.bind(SDLK_w, e_player_up);
			// Down
			Event e_player_down("player.set_direction");
			e_player_down.set("direction", "down");
			keymap.bind(SDLK_s, e_player_down);
			// Left
			Event e_player_left("player.set_direction");
			e_player_left.set("direction", "left");
			keymap.bind(SDLK_a, e_player_left);
			// Right
			Event e_player_right("player.set_direction");
			e_player_right.set("direction", "right");
			keymap.bind(SDLK_d, e_player_right);

			// Register events
			ebus.subscribe("engine.quit", &f_quit);
			ebus.subscribe("player.set_direction", &f_set_dir);

			while(running){
				auto now = SDL_GetTicks();
				const Uint8* state = SDL_GetKeyboardState(nullptr);


				/* Update player */
				//delta.x = 0;
				//delta.y = 0;

				EPlayerDirection pastPlayerDir = playerDir;

				//
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
				/*
				if (state[SDL_SCANCODE_UP])		{ delta.y = -1; playerDir = EPlayerDirection::UP; }
				if (state[SDL_SCANCODE_DOWN])	{ delta.y = 1;	playerDir = EPlayerDirection::DOWN; }
				if (state[SDL_SCANCODE_LEFT])	{ delta.x = -1; playerDir = EPlayerDirection::LEFT; }
				if (state[SDL_SCANCODE_RIGHT])  { delta.x = 1;	playerDir = EPlayerDirection::RIGHT; }
				*/

				/* Change player sprite animation*/
				if (pastPlayerDir != playerDir) {
					switch (playerDir) {
						case EPlayerDirection::UP:
								player.switch_to_anim("up");
							break;
						case EPlayerDirection::DOWN:
								player.switch_to_anim("down");
							break;
						case EPlayerDirection::LEFT:
								player.switch_to_anim("left");
							break;
						case EPlayerDirection::RIGHT:
								player.switch_to_anim("right");
							break;
					}
				}

				/* Normalize delta length */
				if (delta.x != 0 || delta.y != 0) {
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

				/* Adjust player sprite transform*/
				player.setTransform(Z_PlaneMeta{ .x = playerCoordinates.x, .y = playerCoordinates.y, .w = 64, .h = 64 });

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
