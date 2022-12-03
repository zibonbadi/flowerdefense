#include "headers.hh"
#include "player.hh"
#include "enemy.hh"
#include "enemypool.hh"
#include "bfs.hh"

int uninit();

Z_RGBA bgcolor = { .r =0x00, .g = 0x00, .b = 0x00 };
Game game((int)SCREEN_WIDTH, (int)SCREEN_HEIGHT, bgcolor);
ResourceManager g_rc(game.getRenderer());
bool g_isPrintEngineDEBUG = false;
bool running = true;

int main(int argc, char* argv[]) {
	DEBUG_MSG("Engine launched!");
	try {
		DEBUG_MSG("Building system...");

		/* Import spritesheet */
		g_rc.import_texture("spritesheet", "./assets/spritesheet.png");

		/* Select tiles from tileset */
		auto grass = g_rc.make_static_sprite_from_texture("tiles.grass", "spritesheet", Z_PlaneMeta{ .u = 32 * 4, .v = 32 * 5, .uw = 32, .vw = 32 }).second;
		auto rose = g_rc.make_static_sprite_from_texture("tiles.rose", "spritesheet", Z_PlaneMeta{ .u = 32 * 5, .v = 32 * 5, .uw = 32, .vw = 32 }).second;
		auto obstacle = g_rc.make_static_sprite_from_texture("tiles.obstacle", "spritesheet", Z_PlaneMeta{ .u = 32 * 7, .v = 32 * 5, .uw = 32, .vw = 32 }).second;


		Player player((SCREEN_WIDTH / 2) - 32, (SCREEN_HEIGHT / 2) - 32 - 200);
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
		
		
		const float spawnTime				= 2.0f;
		const float spawnTimeWaveEndDelta	= -0.2f;
		const float spawnTimeMin			= 0.2f;
		const float spawnCount				= 1;
		const float poolSize				= 100;
		const float waveTime				= 6.f;

		Enemypool enemyPool(board, collide_enemy, spawnTime, spawnCount, poolSize);

		float waveTimer = waveTime;

		DEBUG_MSG("Entering main loop...");

		// Gloptastic tracker beats
		if(g_rc.import_mod("bgm", "./assets/cycle.stm") != nullptr){
			game.load_mod(g_rc.get_mod("bgm"), -1, -1);
		}


		/* Set up control events */
		EBus_Fn f_quit = [&](Event* e) {
			if (e->get("status_edge") == "up") {
				DEBUG_MSG("Quitting engine on behalf of event " << e);
				running = false;
			}
		};

		EBus_Fn f_print_debug = [&](Event* e) {
			if (e->get("status_edge") == "up") {
				g_isPrintEngineDEBUG = !g_isPrintEngineDEBUG;
				DEBUG_MSG("Engine Debugging " << (g_isPrintEngineDEBUG ? "enabled" : "disabled"));
			}
		};
		EBus_Fn f_restart = [&](Event* e) {
			if (e->get("status_edge") == "up") {
				Event e_up_to_toggle("debug.gameover.toggle");
				e_up_to_toggle.set("status_edge", "toggle");
				g_eventbus.send(&e_up_to_toggle);
				DEBUG_MSG("restarting..");

			} else if (e->get("status_edge") == "toggle") {
				hud.text->visible = !hud.text->visible;
			};
		};

		EBus_Fn f_toggle_spritebox = [&](Event* e) {
			if (e->get("status_edge") == "up") {
				Event e_up_to_toggle("debug.spritebox.toggle");
				e_up_to_toggle.set("status_edge", "toggle");
				g_eventbus.send(&e_up_to_toggle);
			} else if (e->get("status_edge") == "toggle") {
				player.GetSprite()->debug_sprite = !player.GetSprite()->debug_sprite;
				for(auto & enemy : enemyPool.enemies){
					enemy->GetSprite()->debug_sprite = !enemy->GetSprite()->debug_sprite;
				}
			};
		};

		EBus_Fn f_toggle_colliders = [&](Event* e) {
			if (e->get("status_edge") == "up") {
				Event e_up_to_toggle("debug.colliders.toggle");
				e_up_to_toggle.set("status_edge", "toggle");
				g_eventbus.send(&e_up_to_toggle);
			} else if (e->get("status_edge") == "toggle") {
				player.GetSprite()->debug_collide = !player.GetSprite()->debug_collide;
				for(auto & enemy : enemyPool.enemies){
					enemy->GetSprite()->debug_collide = !enemy->GetSprite()->debug_collide;
				}
			};
		};

		// Create Keymap Quit event
		Event e_quit("engine.quit");
		Event e_restart("debug.gameover.toggle");
		Event e_debug_spritebox_toggle("debug.spritebox.toggle");
		Event e_debug_colliders_toggle("debug.colliders.toggle");
		Event e_print_debug("print.debug");
		Event e_debug_playerdie("player.die");

		g_keymapper.bind(SDLK_q, e_quit);
		g_keymapper.bind(SDLK_r, e_restart);
		g_keymapper.bind(SDLK_F1, e_debug_spritebox_toggle);
		g_keymapper.bind(SDLK_F2, e_debug_colliders_toggle);
		g_keymapper.bind(SDLK_F9, e_print_debug);
		g_keymapper.bind(SDLK_F12, e_debug_playerdie);

		// Register events
		g_eventbus.subscribe("engine.quit", &f_quit);
		g_eventbus.subscribe("debug.gameover.toggle", &f_restart);
		g_eventbus.subscribe("debug.spritebox.toggle", &f_toggle_spritebox);
		g_eventbus.subscribe("debug.colliders.toggle", &f_toggle_colliders);
		g_eventbus.subscribe("print.debug", &f_print_debug);

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

			for (int i = 0; i < enemyPool.enemies.size(); i++)
			{
				enemyPool.enemies[i]->Update(bfs.bfsArrows);
			}

			player.Update(deltaTime, enemyPool.enemies);

			waveTimer -= deltaTime;
			if (waveTimer < 0)
			{
				enemyPool._spawnTime += spawnTimeWaveEndDelta;
				if (enemyPool._spawnTime < spawnTimeMin)
				{
					enemyPool._spawnTime = spawnTimeMin;
				}
				waveTimer = waveTime;
			}
			hud.Update(player);


			enemyPool.Update(deltaTime);
			/* Advance the player animation */
			g_rc.advance_all_anim(now);
			game.render();
			past = now;
		}
	}
	catch (std::exception& e) {
		std::cerr << "Exception caught: " << e.what() << std::endl;
	}
	DEBUG_MSG("Main loop stopped.");
	DEBUG_MSG("Shutting down...");
	uninit();
	DEBUG_MSG("Engine stopped!");
	return 0;
}

int uninit() {
	IMG_Quit();
	SDL_Quit();
	return 0;
}

