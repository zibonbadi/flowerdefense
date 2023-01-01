#include "headers.hh"

int uninit();

Z_RGBA bgcolor = { .r =0x00, .g = 0x00, .b = 0x00 };
Game g_game((int)SCREEN_WIDTH, (int)SCREEN_HEIGHT, bgcolor);
ResourceManager g_rc(g_game.getRenderer());
bool g_isPrintEngineDEBUG = false;
bool running = true;
// projectilePool
constexpr int projectilePoolSize = 10;
const float projectileSpeed = 150.f;
const float flowerShotTime = 1.5f;
SDL_FPoint projectileSpawn = { .x = SCREEN_WIDTH / 2, .y = SCREEN_HEIGHT / 2 };
SDL_FPoint* projectilesMoveVec = new SDL_FPoint[projectilePoolSize];
Sprite* projectTileSprites[projectilePoolSize];
float flowerShotTimer = flowerShotTime;
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
		auto preMapObstacle = g_rc.make_static_sprite_from_texture("tiles.preMapObstacle", "spritesheet", Z_PlaneMeta{ .u = 32 * 7, .v = 32 * 4, .uw = 32, .vw = 32 }).second;


		Player player((SCREEN_WIDTH / 2) - 32, (SCREEN_HEIGHT / 2) - 64, 1.5f);
		Tilemap ground(32, 32)/*, plants(32, 32)*/, obstacles;

		// Set backup color for rose tile
		rose->set_color(Z_RGBA{ .r = 0xff, .g = 0x7f, .b = 0xcf, .a = 0xff });
		rose->setTransform(Z_PlaneMeta({ .x = (SCREEN_WIDTH / 2) - 16 , .y = (SCREEN_HEIGHT / 2) - 16, .w = 32, .h = 32 }));
		ground.add_tile('.', grass);
		//plants.add_tile('%', rose);
		obstacles.add_tile('x', obstacle);
		obstacles.add_tile('X', preMapObstacle);

		ground.fill(0, 0, 25, 25, '.');

		/* Game board */
		Plane board(Z_PlaneMeta{ .x = 0, .y = 0, .w = 800, .h = 800 });
		Plane hud_plane(Z_PlaneMeta{ .x = 0, .y = 0, .w = 800, .h = 800 });



		BFS bfsPlayer(obstacles, "bfsPlayer.");

		BFS bfsFlower(obstacles, "bfsFlower.");
		bfsFlower.execute(25, 25);
		float flowerBFSrandomizerCooldownTime = 0.5f;
		float flowerBFSrandomizerCooldownTimer = flowerBFSrandomizerCooldownTime;

		// Construct scene planes
		board.attach(&ground);
		//board.attach(&plants);
		board.attach(&obstacles);
		board.attach(rose);
		board.attach(player.attack);
		board.attach(player.GetSprite());

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
			.x = 4,
			.y = 4,
			.w = 24,
			.h = 24
		},
		collide_rose{
			.x = 0,
			.y = 0,
			.w = 32,
			.h = 32
		},
		collide_projectile{
		.x = 16,
		.y = 16,
		.w = 2,
		.h = 2
		};

		rose->setCollider(&collide_rose);
		player.GetSprite()->setCollider(&collide_player);


		Enemypool enemyPool(board, player, collide_enemy, hud);
		
		auto projPrefix = std::string("projectile.");
		for (int i = 0; i < projectilePoolSize; i++)
		{
			projectilesMoveVec[i] = { .x = -INFINITY, .y = -INFINITY };
			projectTileSprites[i] = g_rc.make_static_sprite_from_texture(projPrefix + std::to_string(i), "spritesheet", Z_PlaneMeta{ .u = 32 * 4, .v = 32 * 4, .uw = 32, .vw = 32 }).second;
			projectTileSprites[i]->setTransform(Z_PlaneMeta{ .x = -INFINITY, .y = -INFINITY });
			projectTileSprites[i]->setCollider(&collide_projectile);
			board.attach(projectTileSprites[i]);
		}

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
				if(g_game.state == EnumGameState::INTRO){
					DEBUG_MSG("Quitting engine on behalf of event " << e);
					running = false;
				}else{
					Event e_reset("game.state.set");
					e_reset.set("scene","gameintro");
					g_eventbus.send(&e_reset);
				}
			}
		};

		EBus_Fn f_print_debug = [&](Event* e) {
			if (e->get("status_edge") == "up") {
				g_isPrintEngineDEBUG = !g_isPrintEngineDEBUG;
				DEBUG_MSG("Engine Debugging " << (g_isPrintEngineDEBUG ? "enabled" : "disabled"));
			}
		};

		EBus_Fn f_game_state_set = [&](Event* e) {
			if(e->get("type") == "game.state.set" && e->get("scene") == "gameintro") {
				hud.ex_rahmen->visible = false;
				hud.tm_inventory->visible = false;
				hud.gameWaveCooldownText->visible = false;
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
			else if (e->get("type") == "game.state.set" && e->get("scene") == "game" &&
			(g_game.state == EnumGameState::INTRO || g_game.state == EnumGameState::GAMEOVER) ){
				// Reset everything
				if(e->get("status_edge") == "down"){
					g_game.play(g_rc.get_sound("restart"));
				}
				DEBUG_MSG("f_game_state_set(e): Caught." << e->get("scene"));
				obstacles.clear_map();

				std::random_device rd;
				std::mt19937 g(rd());
				auto dist = std::uniform_int_distribution<int>(0, 1);
				auto dist2 = std::uniform_int_distribution<int>(6, 18);
				auto dist3 = std::uniform_int_distribution<int>(9, 15);

				int stillToPlace = 20;
				int x, y;
				bool innerFence;
				
				while (stillToPlace > 0) {
					if (dist(g)) {
						innerFence = dist(g);
						if (dist(g)) {
							// linker zaun
							x = innerFence ? 9 : 6;
						}
						else {
							// rechter zaun
							x = innerFence ? 15 : 18;
						}
						y = innerFence ? dist3(g) : dist2(g);
					} else {
						innerFence = dist(g);
						if (dist(g)) {
							// oberer zaun
							y = innerFence ? 9 : 6;
						}
						else {
							// unterer zaun
							y = innerFence ? 15: 18;
						}
						x = innerFence ? dist3(g) : dist2(g);
					}

					if (obstacles.get_spot(x, y) == 'X')
						continue;

					obstacles.fill(x, y, 1, 1, 'X');
					stillToPlace--;
				};

				bfsPlayer.execute(10, 10);
				bfsFlower.execute(25, 25);

				hud.rose_akt_leben = 50;
				hud.rose_max_leben = 50;
				hud.rose_update_health();
				player.reset((SCREEN_WIDTH / 2) - 32, (SCREEN_HEIGHT / 2) - 32 - 200);
				
				/* Enemypool */
				enemyPool.reset();

				hud.ex_rahmen->visible = true;
				hud.tm_inventory->visible = true;
				hud.gameWaveCooldownText->visible = true;

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
				for (int i = 0; i < projectilePoolSize; i++)
				{
					projectTileSprites[i]->debug_collide = debugColliderState;
				}
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
					DEBUG_MSG("Collect obstacle");
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
						bfsFlower.execute(25, 25);
						
						if (bfsFlower.bfsArrows->get_spot(0, 0) == '\0') {
							// no more bfs routes for enemies
							// undo placing operation
							obstacles.fill(_obstacles_target.second, _obstacles_target.first, 1, 1, ' ');
							player.obstacles++;
						}
						else {
							// bfs is still working
							// finish placing operation with sound
							g_game.play(g_rc.get_sound("fence.place"));
						}
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

		g_keymapper.bind(SDLK_ESCAPE, &e_quit);
		g_keymapper.bind(SDLK_RETURN, &e_game_state_set);
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
				break;
				}
				case SDL_MOUSEBUTTONDOWN:
				player.press = true;
				player.mouseX = e.button.x;
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
				enemyPool.Update(deltaTime);


				int x_tile = ((int)player.playerCoordinates.x + 32 - 16) / BFS_TILE_WIDTH;
				int y_tile = ((int)player.playerCoordinates.y + 32 + 24) / BFS_TILE_HEIGHT;

				bfsPlayer.execute(x_tile, y_tile);

				flowerBFSrandomizerCooldownTimer -= deltaTime;
				if (flowerBFSrandomizerCooldownTimer < 0) {
					bfsFlower.execute(25, 25);

					flowerBFSrandomizerCooldownTimer = flowerBFSrandomizerCooldownTime;
				}

				for (int i = 0; i < enemyPool.enemies.size(); i++)
				{
					enemyPool.enemies[i]->Update(bfsFlower, bfsPlayer, &obstacles);
				}
			}

			if (g_game.state == EnumGameState::PLAY) {
				
				/* Flower shooting */

				// Spawn projectiles
				flowerShotTimer -= deltaTime;
				if (flowerShotTimer < 0) {
					flowerShotTimer = flowerShotTime;

					float min = INFINITY;
					int minIndex = -1;
					for (int i = 0; i < enemyPool._poolSize; i++)
					{
						Enemy* e = enemyPool.enemies[i];
						if (e->visible && !e->isdead && e->_flightPathRoseLength < min) {
							min = e->_flightPathRoseLength;
							minIndex = i;
						}
					}

					if (min != INFINITY) {

						Enemy* nearestEnemy = enemyPool.enemies[minIndex];


						// projectileSpawn


						constexpr int projectileSpawnPointX = (SCREEN_WIDTH / 2) + 16;
						constexpr int projectileSpawnPointY = (SCREEN_HEIGHT / 2) + 16;

						SDL_FPoint flighPathNearestEnemy = {
							.x = (nearestEnemy->coordinates.x + 16) - projectileSpawnPointX,
							.y = (nearestEnemy->coordinates.y + 16) - projectileSpawnPointY,
						};
						//float angle = (atan2(-flighPathNearestEnemy.x, flighPathNearestEnemy.y) * 180 / M_PI) + 180;
						//DEBUG_MSG((int)angle << "°");
						for (int i = 0; i < projectilePoolSize; i++)
						{
							auto zpMeta = projectTileSprites[i]->get_transform();
							if (zpMeta.x < 0 ||
								zpMeta.x >= SCREEN_WIDTH ||
								zpMeta.y < 0 ||
								zpMeta.y >= SCREEN_HEIGHT) {


								// projectTileSprites[i] ist frei

								// spawn

									/* Normalize delta length */
								if (flighPathNearestEnemy.x != 0 || flighPathNearestEnemy.y != 0) {
									const float length_inverse = 1.f / sqrt(flighPathNearestEnemy.x * flighPathNearestEnemy.x + flighPathNearestEnemy.y * flighPathNearestEnemy.y);
									flighPathNearestEnemy.x *= length_inverse;
									flighPathNearestEnemy.y *= length_inverse;
								}

								projectilesMoveVec[i] = { .x = flighPathNearestEnemy.x, .y = flighPathNearestEnemy.y };
								DEBUG_MSG(projectilesMoveVec[i].x << " " << projectilesMoveVec[i].y)
								projectTileSprites[i]->setTransform(Z_PlaneMeta{ .x = projectileSpawnPointX - 32, .y = projectileSpawnPointY - 32, .w = 32, .h = 32 });
								break;
							}
						}
					}
				}

			}

			// Update projectiles
			for (int i = 0; i < projectilePoolSize; i++)
			{
				auto zpMeta = projectTileSprites[i]->get_transform();
				if (zpMeta.x >= -32 &&
					zpMeta.x <= SCREEN_WIDTH &&
					zpMeta.y >= -32 &&
					zpMeta.y <= SCREEN_HEIGHT) {


					float deltaX = projectilesMoveVec[i].x * projectileSpeed * deltaTime;
					float deltaY = projectilesMoveVec[i].y * projectileSpeed * deltaTime;
					projectTileSprites[i]->setTransform(Z_PlaneMeta{ .x = zpMeta.x + deltaX, .y = zpMeta.y + deltaY, .w = 32, .h = 32 });

					for (Enemy* e : enemyPool.enemies) {
						if (e->visible && !e->isdead && projectTileSprites[i]->collision(e->GetSprite())) {
							if (!e->isdead && e->visible) {
								e->dying();
							}
						}
					}
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
	delete[] projectilesMoveVec;
	uninit();
	DEBUG_MSG("Engine stopped!");
	return 0;
}

int uninit() {
	IMG_Quit();
	SDL_Quit();
	return 0;
}

