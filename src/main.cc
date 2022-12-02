#include "headers.hh"
#include "player.hh"
#include "enemy.hh"
#include "enemypool.hh"
#include "bfs.hh"

int uninit();
int update(Tilemap* tmap);
int poll();
bool running = true;

Z_RGBA bgcolor = { .r =0x00, .g = 0x00, .b = 0x00 };
Game game((int)SCREEN_WIDTH, (int)SCREEN_HEIGHT, bgcolor);
ResourceManager g_rc(game.getRenderer());

int main(int argc, char* argv[]) {
	std::cout << "Engine launched!" << std::endl;
	try {
		std::cout << "Building system..." << std::endl;


		//damit gegner nur einmal Stirbt 
		int i = 0;

		/* Import spritesheet */
		g_rc.import_texture("spritesheet", "./assets/spritesheet.png");

		/* Select tiles from tileset */
		auto grass = g_rc.make_static_sprite_from_texture("tiles.grass", "spritesheet", Z_PlaneMeta{ .u = 32 * 4, .v = 32 * 5, .uw = 32, .vw = 32 }).second;
		auto rose = g_rc.make_static_sprite_from_texture("tiles.rose", "spritesheet", Z_PlaneMeta{ .u = 32 * 5, .v = 32 * 5, .uw = 32, .vw = 32 }).second;
		auto obstacle = g_rc.make_static_sprite_from_texture("tiles.obstacle", "spritesheet", Z_PlaneMeta{ .u = 32 * 7, .v = 32 * 5, .uw = 32, .vw = 32 }).second;


		Player player((SCREEN_WIDTH / 2) - 32, (SCREEN_HEIGHT / 2) - 32 - 200);
		Enemy enemy(16*5, 16 * 45, "1");
		Enemy enemy2(16 * 24, 16 * 13, "2");
		Enemy enemy3(16 * 10, 16 * 35, "3");

		Enemypool enemyPool(3);
		std::vector<Enemy*> enemies;
		enemies.push_back(&enemy);
		enemies.push_back(&enemy2);
		enemies.push_back(&enemy3);
		//Enemy enemy(0, 0 ebus, keymap);

		Tilemap ground(32, 32), plants(32, 32), obstacles;
		

		// Set backup color for rose tile
		rose->set_color(Z_RGBA{ .r = 0xff, .g = 0x7f, .b = 0xcf, .a = 0xff });

		ground.add_tile('.', grass);
		plants.add_tile('%', rose);
		obstacles.add_tile('x', obstacle);

		ground.fill(0, 0, 25, 25, '.');
		plants.fill(12, 12, 1, 1, '%');
		obstacles.fill(4, 9, 4, 1, 'x');

		

		/* Game board */
		Plane board(Z_PlaneMeta{ .x = 0, .y = 0, .w = 800, .h = 800 });
		Plane hud_plane(Z_PlaneMeta{ .x = 0, .y = 0, .w = 800, .h = 800 });

		BFS bfs(board, obstacles);
		bfs.execute(10, 10);

		// Construct scene planes
		board.attach(&ground);
		board.attach(&plants);
		board.attach(&obstacles);
		board.attach(player.GetSprite());
		board.attach(enemy.GetSprite());
		board.attach(enemy2.GetSprite());
		board.attach(enemy3.GetSprite());

		Hud hud(hud_plane);
		//hud.exp_create(5,8);

		// Hook plane into scene
		game.attach(&board);
		game.attach(&hud_plane);

		Z_PlaneMeta collide_player{
			.x = 0,
			.y = 0,
			.w = 64,
			.h = 64
		},
		collide_enemy{
			.x = 0,
			.y = 0,
			.w = 32,
			.h = 32
		};

		player.GetSprite()->setCollider(&collide_player);
		enemy.GetSprite()->setCollider(&collide_enemy);
		enemy2.GetSprite()->setCollider(&collide_enemy);
		enemy3.GetSprite()->setCollider(&collide_enemy);

		std::cout << "Entering main loop..." << std::endl;

		// Gloptastic tracker beats
		if(g_rc.import_mod("bgm", "./assets/cycle.stm") != nullptr){
			game.load_mod(g_rc.get_mod("bgm"), -1, -1);
		}


		/* Set up control events */
		EBus_Fn f_quit = [&](Event* e) {
			if (e->get("status_edge") == "up") {
				std::clog << "Quitting engine on behalf of event " << e << std::endl;
				running = false;
			};
		};

		EBus_Fn f_debug_collide = [&](Event* e) {
			if (e->get("status_edge") == "down") {
				bool sfdfs = player.GetSprite()->collision(enemy.GetSprite());
				std::cout 
					<< "Collision player->enemy (bool):" << player.GetSprite()->collision(enemy.GetSprite()) << std::endl
					<< "Collision enemy->player (bool):" << enemy.GetSprite()->collision(player.GetSprite()) << std::endl
					<< "Collision player->enemy2 (bool):" << player.GetSprite()->collision(enemy2.GetSprite()) << std::endl
					<< "Collision enemy2->player (bool):" << enemy2.GetSprite()->collision(player.GetSprite()) << std::endl
				;
			};
		};

		// Create Keymap Quit event
		Event e_quit("engine.quit");
		Event e_debug_collide("debug.collide");
		g_keymapper.bind(SDLK_q, e_quit);
		g_keymapper.bind(SDLK_c, e_debug_collide);

		// Register events
		g_eventbus.subscribe("engine.quit", &f_quit);
		g_eventbus.subscribe("debug.collide", &f_debug_collide);


		uint32_t past = 0;
		while (running) {
			auto now = SDL_GetTicks();
			const Uint8* state = SDL_GetKeyboardState(nullptr);
			auto deltaMilliseconds = now - past;
			float deltaTime = (float)deltaMilliseconds / 1000;

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
					g_keymapper.probe(e.key);
				}
				default: {
					break;
				}
				}
			}

			enemy.Update(bfs.bfsArrows);
			enemy2.Update(bfs.bfsArrows);
			enemy3.Update(bfs.bfsArrows);


			if(i == 0){
				//test zum toten Gegner
				enemy3.dying();
				//enemy3.disappear();
				//enemy3.reborn(16*24, 16 * 20);
				i++;
			}


			player.Update(deltaTime, enemies);

			/* Advance the player animation */
			g_rc.advance_all_anim(now);
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
