#include "headers.hh"

int uninit();

Z_RGBA bgcolor = { .r =0x00, .g = 0x00, .b = 0x00 };
Game g_game((int)SCREEN_WIDTH, (int)SCREEN_HEIGHT, bgcolor);
ResourceManager g_rc(g_game.getRenderer());
bool g_isPrintEngineDEBUG = false;
bool running = true;

int main(int argc, char* argv[]) {

	LVLSystem lvlS;


	DEBUG_MSG("Engine launched!");
	try {
		DEBUG_MSG("Building system...");

		/* Import spritesheet */
		g_rc.import_texture("spritesheet", "./assets/spritesheet.png");
		g_rc.import_texture("lvlUp_option", "./assets/lvlUp_option.png");


		/* Select tiles from tileset */
		auto grass = g_rc.make_static_sprite_from_texture("tiles.grass", "spritesheet", Z_PlaneMeta{ .u = 32 * 4, .v = 32 * 5, .uw = 32, .vw = 32 }).second;
		auto rose = g_rc.make_static_sprite_from_texture("tiles.rose", "spritesheet", Z_PlaneMeta{ .u = 32 * 5, .v = 32 * 5, .uw = 32, .vw = 32 }).second;
		auto obstacle = g_rc.make_static_sprite_from_texture("tiles.obstacle", "spritesheet", Z_PlaneMeta{ .u = 32 * 7, .v = 32 * 5, .uw = 32, .vw = 32 }).second;


		Player player((SCREEN_WIDTH / 2) - 16, (SCREEN_HEIGHT / 2) - 32 - 200, 1.5f);
		Tilemap ground(32, 32)/*, plants(32, 32)*/, obstacles;

		// Set backup color for rose tile
		rose->set_color(Z_RGBA{ .r = 0xff, .g = 0x7f, .b = 0xcf, .a = 0xff });
		rose->setTransform(Z_PlaneMeta({ .x = (SCREEN_WIDTH / 2) - 16 , .y = (SCREEN_HEIGHT / 2) - 16, .w = 32, .h = 32 }));
		ground.add_tile('.', grass);
		//plants.add_tile('%', rose);
		obstacles.add_tile('x', obstacle);

		ground.fill(0, 0, 25, 25, '.');

		/* Game board */
		Plane board(Z_PlaneMeta{ .x = 0, .y = 0, .w = 800, .h = 800 });
		Plane hud_plane(Z_PlaneMeta{ .x = 0, .y = 0, .w = 800, .h = 800 });



		BFS bfsPlayer(obstacles, "bfsPlayer.");
		bfsPlayer.execute(10, 10);

		BFS bfsFlower(obstacles, "bfsFlower.");
		bfsFlower.execute(25, 25);

		// Construct scene planes
		board.attach(&ground);
		//board.attach(&plants);
		board.attach(&obstacles);
		board.attach(rose);
		board.attach(player.GetSprite());
		board.attach(player.attack);

		Hud hud(hud_plane);
		//hud.exp_create(5,8);
		// Hook plane into scene
		g_game.attach(&board);
		g_game.attach(&hud_plane);

		Z_PlaneMeta collide_player{
			.x = 0,
			.y = 24,
			.w = 32,
			.h = 40
		},
		collide_enemy{
			.x = 0,
			.y = 0,
			.w = 32,
			.h = 32
		},
		collide_rose{
			.x = 0,
			.y = 0,
			.w = 32,
			.h = 32
		};

		rose->setCollider(&collide_rose);
		player.GetSprite()->setCollider(&collide_player);

		
		const float spawnTime = 2.0f;
		const float spawnTimeDeltaAtWaveEnd = -0.2f;
		const float spawnTimeMin = 0.2f;
		const float spawnCount = 1;
		const float poolSize = 100;
		const float waveTime = 6.f;
		Enemypool enemyPool(board, player, collide_enemy, spawnTime, spawnCount, poolSize);

		float waveTimer = waveTime;

		DEBUG_MSG("Entering main loop...");

		// Gloptastic tracker beats
		if(g_rc.import_mod("bgm", "./assets/cycle.stm") != nullptr){
			g_game.load_mod(g_rc.get_mod("bgm"), -1, -1);
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

		EBus_Fn f_game_state_set = [&](Event* e) {
			if (e->get("type") == "game.state.set" && e->get("scene") == "gameintro") {
				hud.ex_rahmen->visible = false;
				hud.tm_inventory->visible = false;
				hud.dettach_rose_leben();
				g_game.state = EnumGameState::INTRO;
			}
			else if (e->get("type") == "game.state.set" && e->get("scene") == "gameover") {
				hud.ex_rahmen->visible = false;
				hud.tm_inventory->visible = false;
				hud.dettach_rose_leben();
				g_game.state = EnumGameState::GAMEOVER;
			}
			else if (e->get("type") == "game.state.set" && e->get("scene") == "game"){
				// Reset everything
				DEBUG_MSG("f_restart(e): Caught." << e->get("scene"));
				obstacles.clear_map();

				obstacles.fill(4, 9, 4, 1, 'x');

				bfsPlayer.execute(10, 10);
				bfsFlower.execute(25, 25);

				hud.rose_akt_leben = 50;
				hud.rose_max_leben = 50;
				hud.rose_update_health();
				player.reset((SCREEN_WIDTH / 2) - 32, (SCREEN_HEIGHT / 2) - 32 - 200);
				enemyPool.reset();

				/* Enemypool */
				enemyPool._spawnTime = spawnTime;
				enemyPool._spawnCount = spawnCount;
				enemyPool._poolSize = poolSize;
				hud.ex_rahmen->visible = true;
				hud.tm_inventory->visible = true;
				hud.attach_rose_leben();
				g_game.state = EnumGameState::PLAY;
			};
		};

		EBus_Fn f_toggle_spritebox = [&](Event* e) {
			if (e->get("status_edge") == "up") {
				player.GetSprite()->debug_sprite = !player.GetSprite()->debug_sprite;
				bool debugSpriteState = player.GetSprite()->debug_sprite;
				player.attack->debug_sprite = debugSpriteState;
				rose->debug_sprite = debugSpriteState;
				for (auto& enemy : enemyPool.enemies) {
					enemy->GetSprite()->debug_sprite = debugSpriteState;
				}
			}
		};

		EBus_Fn f_toggle_colliders = [&](Event* e) {
			if (e->get("status_edge") == "up") {
				player.GetSprite()->debug_collide = !player.GetSprite()->debug_collide;
				bool debugColliderState = player.GetSprite()->debug_collide;
				player.attack->debug_collide = debugColliderState;
				rose->debug_collide = debugColliderState;
				for (auto& enemy : enemyPool.enemies) {
					enemy->GetSprite()->debug_collide = debugColliderState;
				}
			}
		};
			
		EBus_Fn f_place_obstacle = [&](Event* e) {
			if (e->get("status_edge") == "up") {
				auto target = obstacles.get_coordinate_from_offset(
					std::stoi(e->get("player.x"))+32 - 16,
					std::stoi(e->get("player.y"))+32 + 20
				);
				DEBUG_MSG("Checking spot(" << target.first << ", " << target.second << ')');
				auto spot = obstacles.get_spot(target.second, target.first);
				switch(spot){
				case 'x': {
					DEBUG_MSG("Removing obstacle");
					obstacles.fill(target.second, target.first, 1, 1, ' ');
					player.obstacles++;
					break;
				}
				case ' ': {
					if( player.obstacles > 0 ){
						DEBUG_MSG("Placing obstacle");
						obstacles.fill(target.second, target.first, 1, 1, 'x');
						player.obstacles--;
					}
					break;
				}
				default: {
					DEBUG_MSG("Found tile \"" << spot << '\"');
				break;
				}
				}
				bfsFlower.execute(25, 25);
			}
		};

		EBus_Fn f_bfs_player_visibility = [&](Event* e) {
			if (e->get("status_edge") == "up") {
				bfsPlayer.isAttached = !bfsPlayer.isAttached;
				if (bfsPlayer.isAttached == true) {
					board.attach(bfsPlayer.bfsArrows);
				}
				else {
					board.detach(bfsPlayer.bfsArrows);
				}
			}
		};

		EBus_Fn f_bfs_flower_visibility = [&](Event* e) {
			if (e->get("status_edge") == "up") {
				bfsFlower.isAttached = !bfsFlower.isAttached;
				if (bfsFlower.isAttached == true) {
					board.attach(bfsFlower.bfsArrows);
				}
				else {
					board.detach(bfsFlower.bfsArrows);
				}
			}
		};

		Event e_bfs_player_visibility("bfs.player_visibility");
		Event e_bfs_flower_visibility("bfs.flower_visibility");

		// Register event

		// Create Keymap Quit event
		Event e_quit("engine.quit");
		Event e_game_state_set("game.state.set");
		e_game_state_set.set("scene", "game");
		e_game_state_set.set("status_edge", "up");
		Event e_debug_spritebox_toggle("debug.spritebox.toggle");
		Event e_debug_colliders_toggle("debug.colliders.toggle");
		Event e_print_debug("print.debug");

		g_keymapper.bind(SDLK_q, &e_quit);
		g_keymapper.bind(SDLK_r, &e_game_state_set);
		g_keymapper.bind(SDLK_F1, &e_debug_spritebox_toggle);
		g_keymapper.bind(SDLK_F2, &e_debug_colliders_toggle);
		g_keymapper.bind(SDLK_F3, &e_bfs_player_visibility);
		g_keymapper.bind(SDLK_F4, &e_bfs_flower_visibility);
		g_keymapper.bind(SDLK_F9, &e_print_debug);

		// Register events
		g_eventbus.subscribe("engine.quit", &f_quit);
		g_eventbus.subscribe("game.state.set", &f_game_state_set);
		g_eventbus.subscribe("player.place_fence", &f_place_obstacle);
		g_eventbus.subscribe("debug.spritebox.toggle", &f_toggle_spritebox);
		g_eventbus.subscribe("debug.colliders.toggle", &f_toggle_colliders);
		g_eventbus.subscribe("print.debug", &f_print_debug);
		g_eventbus.subscribe("bfs.player_visibility", &f_bfs_player_visibility);
		g_eventbus.subscribe("bfs.flower_visibility", &f_bfs_flower_visibility);


		// Launch game into intro state
		Event e_start("game.state.set");
		e_start.set("scene", "gameintro");
		e_start.set("status_edge", "up");
		g_eventbus.send(&e_start);

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
				case SDL_MOUSEBUTTONDOWN:
				player.press = true;
				player.mouseY = e.button.y;
				break;

				default: {
					break;
				}
				}
			}
			hud.Update(player, enemyPool);
			lvlS.Update(player, enemyPool);

			

			if(g_game.state != EnumGameState::SKILLSELECT){
			player.Update(deltaTime, enemyPool.enemies, rose, hud);


			waveTimer -= deltaTime;
			if (waveTimer < 0)
			{
				enemyPool._spawnTime += spawnTimeDeltaAtWaveEnd;
				if (enemyPool._spawnTime < spawnTimeMin)
				{
					enemyPool._spawnTime = spawnTimeMin;
				}
				hud.wave++;
				waveTimer = waveTime;
			}

			

			enemyPool.Update(deltaTime);

			int x_tile = ((int)player.playerCoordinates.x + 32 - 16) / BFS_TILE_WIDTH;
			int y_tile = ((int)player.playerCoordinates.y + 32 + 24) / BFS_TILE_HEIGHT;
			
			bfsPlayer.execute(x_tile, y_tile);

			for (int i = 0; i < enemyPool.enemies.size(); i++)
			{
				enemyPool.enemies[i]->Update(bfsFlower, bfsPlayer);
			}
		}
			/* Advance the player animation */
			g_rc.advance_all_anim(now);
			g_game.render();
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

