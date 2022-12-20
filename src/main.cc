#include "headers.hh"

int uninit();

Z_RGBA bgcolor = { .r =0x00, .g = 0x00, .b = 0x00 };
Game g_game((int)SCREEN_WIDTH, (int)SCREEN_HEIGHT, bgcolor);
ResourceManager g_rc(g_game.getRenderer());
bool g_isPrintEngineDEBUG = false;
bool running = true;

int main(int argc, char* argv[]) {

	LVLSystem lvlS;

	SDL_version sdl_version;
	SDL_GetVersion(&sdl_version);
	DEBUG_MSG("SDL VERSION: " << ((int)sdl_version.major) << "." << ((int)sdl_version.minor) << "." << ((int)sdl_version.patch));
	DEBUG_MSG("MIXER VERSION: " << ((int)Mix_Linked_Version()->major) << "." << ((int)Mix_Linked_Version()->minor) << "." << ((int)Mix_Linked_Version()->patch));
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


		Player player((SCREEN_WIDTH / 2) - 32, (SCREEN_HEIGHT / 2) - 64, 1.5f);
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
		const float waveTime = 18.f;
		const float waveCoolDownTime = 6.0f;
		Enemypool enemyPool(board, player, collide_enemy, spawnTime, spawnCount, poolSize);

		float waveTimer = waveTime;
		float waveCoolDownTimer = -1.0f;

		DEBUG_MSG("Entering main loop...");

		// Gloptastic tracker beats
		if(g_rc.import_mod("bgm", "./assets/liftyasoul.xm") != nullptr){
			g_game.load_mod(g_rc.get_mod("bgm"), -1, -1);
		}

		g_rc.import_sound("restart", "./assets/restart.wav");
		g_rc.import_sound("bee.spawn", "./assets/spawn_bee.wav");
		g_rc.import_sound("player.lvlup", "./assets/lvlup.wav");
		g_rc.import_sound("fence.place", "./assets/place_fence.wav");
		g_rc.import_sound("fence.select", "./assets/select.wav");
		g_rc.import_sound("enemy.kill", "./assets/kill.wav");
		g_rc.import_sound("player.damage", "./assets/damage.wav");
		g_rc.import_sound("player.pickup.xp", "./assets/pickup_xp.wav");


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
				if (g_game.state == EnumGameState::INTRO) {
					return; // let the intro just show the intro text, and not the game over text
				}

				hud.ex_rahmen->visible = false;
				hud.tm_inventory->visible = false;
				hud.dettach_rose_leben();
				g_game.state = EnumGameState::GAMEOVER;
			}
			else if (e->get("type") == "game.state.set" && e->get("scene") == "game"){
				// Reset everything
				if(e->get("status_edge") == "down"){
					g_game.play(g_rc.get_sound("restart"));
				}
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
				waveTimer = waveTime;
				waveCoolDownTimer = -1.0f;

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
			
		bool tileHL = false;
		SDL_Rect tile_hl_rect;
		auto _obstacles_target = obstacles.get_coordinate_from_offset(
			player.playerCoordinates.x+32 - 16,
			player.playerCoordinates.y+32 + 20
		);
		auto _obstacles_spot = obstacles.get_spot(_obstacles_target.second, _obstacles_target.first);
		auto _obstacles_grit = obstacles.get_grit();

		EBus_Fn f_place_obstacle = [&](Event* e) {
			DEBUG_MSG("Checking spot(" << _obstacles_target.first << ", " << _obstacles_target.second << ')');

				_obstacles_target = obstacles.get_coordinate_from_offset(
					player.playerCoordinates.x+32 - 16,
					player.playerCoordinates.y+32 + 20
				);
				_obstacles_spot = obstacles.get_spot(_obstacles_target.second, _obstacles_target.first);
				_obstacles_grit = obstacles.get_grit();
				tile_hl_rect.x = _obstacles_target.first;
				tile_hl_rect.y = _obstacles_target.second;
				tile_hl_rect.w = _obstacles_grit.first;
				tile_hl_rect.h = _obstacles_grit.second;

			if (e->get("status_edge") == "up") {
				switch(_obstacles_spot){
				case 'x': {
					DEBUG_MSG("Removing obstacle");
					obstacles.fill(_obstacles_target.second, _obstacles_target.first, 1, 1, ' ');
					player.obstacles++;
					g_game.play(g_rc.get_sound("fence.place"));
					break;
				}
				case ' ': {
					if( player.obstacles > 0 ){
						DEBUG_MSG("Placing obstacle");
						obstacles.fill(_obstacles_target.second, _obstacles_target.first, 1, 1, 'x');
						player.obstacles--;
						g_game.play(g_rc.get_sound("fence.place"));
					}
					break;
				}
				default: {
					DEBUG_MSG("Found tile \"" << _obstacles_spot << '\"');
					break;
				}
				}
				bfsFlower.execute(25, 25);
				tileHL = false;
			}
			if (e->get("status_edge") == "down") {
				DEBUG_MSG("Highlight Rect properties: { .x = " << tile_hl_rect.x << ", .y = " << tile_hl_rect.y <<  ", .w = " << tile_hl_rect.w <<  ", .h = " << tile_hl_rect.h << " }" );
				g_game.play(g_rc.get_sound("fence.select"));
				tileHL = true;
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
			lvlS.Update(player, enemyPool, hud);

			

			if(g_game.state != EnumGameState::SKILLSELECT){
				player.Update(deltaTime, enemyPool.enemies, rose, hud);

				waveCoolDownTimer -= deltaTime;
				

				if (waveCoolDownTimer < 0) {
					hud.gameWaveCooldownText->visible = false;
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
						waveCoolDownTimer = waveCoolDownTime;
						if (g_game.state == EnumGameState::PLAY) {
							hud.gameWaveCooldownText->visible = true;
						}
					}



					enemyPool.Update(deltaTime);
				}
				else {
					hud.gameWaveCooldownText->write(std::pair(18, 5), "Next Wave in\r\n"+std::to_string((int)ceil(waveCoolDownTimer)) + " Seconds");
				}

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

			/* Janky Tile highlights */
			if(tileHL){
				ENGINE_DEBUG_MSG("Rendering tileHL...")
				_obstacles_target = obstacles.get_coordinate_from_offset(
					player.playerCoordinates.y+32 + 20,
					player.playerCoordinates.x+32 - 16
				);
				_obstacles_spot = obstacles.get_spot(_obstacles_target.first, _obstacles_target.second);
				_obstacles_grit = obstacles.get_grit();
				tile_hl_rect.x = _obstacles_target.first*_obstacles_grit.first;
				tile_hl_rect.y = _obstacles_target.second*_obstacles_grit.second;
				tile_hl_rect.w = _obstacles_grit.first;
				tile_hl_rect.h = _obstacles_grit.second;

				SDL_SetRenderDrawColor(g_game.getRenderer(), 0xcf, 0xcf, 0xcf, 0x7f );
				SDL_RenderFillRect(g_game.getRenderer(), &tile_hl_rect);
			}

			SDL_RenderPresent(g_game.getRenderer());

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

