#include "headers.hh"
#include "player.hh"
#include "enemy.hh"
#include "bfs.hh"

int uninit();
int update(Tilemap* tmap);
int poll();
bool running = true;

int main(int argc, char* argv[]) {
	std::cout << "Engine launched!" << std::endl;
	try {
		std::cout << "Building system..." << std::endl;
		Z_RGBA bgcolor = { .r = 0x00, .g = 0x00, .b = 0x00 };

		Game game((int)SCREEN_WIDTH, (int)SCREEN_HEIGHT, bgcolor);

		EventBus ebus;
		KeyMapper keymap(&ebus);

		ResourceManager rc(game.getRenderer());
		/* Import spritesheet */
		rc.import_texture("spritesheet", "./assets/spritesheet.png");

		/* Select tiles from tileset */
		auto grass = rc.make_static_sprite_from_texture("tiles.grass", "spritesheet", Z_PlaneMeta{ .u = 32 * 4, .v = 32 * 5, .uw = 32, .vw = 32 }).second;
		auto rose = rc.make_static_sprite_from_texture("tiles.rose", "spritesheet", Z_PlaneMeta{ .u = 32 * 5, .v = 32 * 5, .uw = 32, .vw = 32 }).second;

		Player player((SCREEN_WIDTH / 2) - 32, (SCREEN_HEIGHT / 2) - 32 - 200, rc, ebus, keymap);
		Enemy enemy(16*20, 16 * 45, rc, ebus);

		//Enemy enemy(0, 0, rc, ebus, keymap);

		Tilemap ground(32, 32), plants(32, 32);


		// Set backup color for rose tile
		rose->set_color(Z_RGBA{ .r = 0xff, .g = 0x7f, .b = 0xcf, .a = 0xff });

		ground.add_tile('.', grass);
		plants.add_tile('%', rose);

		ground.fill(0, 0, 25, 25, '.');
		plants.fill(12, 12, 1, 1, '%');

		

		/* Game board */
		Plane board(Z_PlaneMeta{ .x = 0, .y = 0, .w = 800, .h = 800 });

		BFS bfs(rc, ebus, keymap, board);
		bfs.execute(10, 10);

		// Construct scene planes
		board.attach(&ground);
		board.attach(&plants);
		board.attach(player.GetSprite());
		board.attach(enemy.GetSprite());

		Hud hud(rc,board);
		hud.exp_create(5,8);

		// Hook plane into scene
		game.attach(&board);


		std::cout << "Entering main loop..." << std::endl;

		// Gloptastic tracker beats
		if(rc.import_mod("bgm", "./assets/cycle.stm") != nullptr){
			game.load_mod(rc.get_mod("bgm"), -1, -1);
		}


		/* Set up control events */
		EBus_Fn f_quit = [&](Event* e) {
			if (e->get("status_edge") == "up") {
				std::clog << "Quitting engine on behalf of event " << e << std::endl;
				running = false;
			};
		};

		// Create Keymap Quit event
		Event e_quit("engine.quit");
		keymap.bind(SDLK_q, e_quit);

		// Register events
		ebus.subscribe("engine.quit", &f_quit);


		uint32_t past = 0;
		while (running) {
			auto now = SDL_GetTicks();
			const Uint8* state = SDL_GetKeyboardState(nullptr);


			// poll Events
			SDL_Event e;
			while (SDL_PollEvent(&e)) {
				switch (e.type) {
				case SDL_QUIT: {
					throw std::runtime_error("Received QUIT signal");
					break;
				}
				case SDL_KEYUP:
				case SDL_KEYDOWN: {
					keymap.probe(e.key);
				}
				default: {
					break;
				}
				}
			}

			enemy.Update(bfs.bfsArrows);
			player.Update();

			/* Advance the player animation */
			rc.advance_all_anim(now);
			game.render();
			past = now;
		}
	}
	catch (std::exception& e) {
		std::cerr << "Exception caught: " << e.what() << std::endl;
	}
	std::cout << "Main loop stopped." << std::endl;
	std::cout << "Shutting down..." << std::endl;
	uninit();
	std::cout << "Engine stopped!" << std::endl;
	return 0;
}

int uninit() {
	IMG_Quit();
	SDL_Quit();
	return 0;
}

int update(Tilemap* tmap) {
	return 0;
};

int poll() {
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		switch (e.type) {
		case SDL_QUIT: {
			throw std::runtime_error("Received QUIT signal");
			break;
		}
		case SDL_KEYDOWN: {
			switch (e.key.keysym.sym) {
			case SDLK_q: {
				throw std::runtime_error("Received QUIT signal from user");
				break;
			}
			case SDLK_UP:
			case SDLK_w: {
				// Game logic
				//current_dir = UP;
				break;
			}
			case SDLK_DOWN:
			case SDLK_s: {
				// Game logic
				//current_dir = DOWN;
				break;
			}
			case SDLK_LEFT:
			case SDLK_a: {
				// Game logic
				//current_dir = LEFT;
				break;
			}
			case SDLK_RIGHT:
			case SDLK_d: {
				// Game logic
				//current_dir = RIGHT;
				break;
			}
			case SDLK_p: {
				//SDL_PauseAudio(adev, 1);
				break;
			}
			default: {
				break;
			}
			}
			break;
		}
		default: {
			break;
		}
		}
	}
	return 0;
};