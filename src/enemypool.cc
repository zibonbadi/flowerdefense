#include "enemypool.hh"
#include <cstdio>      /* srand, rand */
#include <ctime>      /* time */

Enemypool::Enemypool(Plane& board, Player& player, Z_PlaneMeta& collide_enemy, float spawnTime, int spawnCount, int poolSize) : _board(board), _player(player), _collide_enemy(collide_enemy), _spawnTime(spawnTime), _spawnCount(spawnCount), _poolSize(poolSize)
{
	//enemies.resize(poolSize);
	//enemies = poolSize;

	initAnimations();

	for (int i = 0; i < poolSize; i++)
	{
		enemies.push_back(new Enemy(-INFINITY, -INFINITY, _player, EEnemyType::BEE, EEnemyDirection::RIGHT));
	}
	
	// Hook handleEvents into event handler
	this->f_eHandler = new EBus_Fn( std::bind(&Enemypool::handleEvents, this, std::placeholders::_1) );
	// Specify event subscriptions
	//g_eventbus.subscribe("game.state.set", f_eHandler);
}

void Enemypool::Update(const float& deltaTime)
{
	_spawnTimer -= deltaTime;
	if (_spawnTimer < 0)
	{
		Spawn(_spawnCount);
		_spawnTimer = _spawnTime;
		Recollect();

	}
}

int Enemypool::getAvailableCount(){
	auto rVal = 0;
	for (auto & e : enemies){
		if (!e->visible){
			rVal++;
		}
	}
	return rVal;
}

Enemy* Enemypool::getFirstAvailable(){
	for (auto & e : enemies){
		if (!e->visible)
			return e;
	}
	return nullptr;
}

void Enemypool::Recollect()
{
	for (auto & e : enemies)
	{
		if (!e->visible) {
			e->reborn(-INFINITY, -INFINITY);
			//availableEnemies.push_back(e);
			//_availableEnemiesSize++;
			//enemies.erase(e);
		}
	}
}

void Enemypool::Spawn(int count)
{
	auto available = getAvailableCount();
	if ((available  - count) < 0)
	{
		std::cerr << "Error in Enemypool: Not enough availableEnemies left to spawn ("
			<< available << '/' << enemies.size() << " left; "
			<< count << " required)" << std::endl;
		return;
	}

	constexpr int TileCountX = SCREEN_WIDTH / BFS_TILE_WIDTH;
	constexpr int TileCountY = SCREEN_HEIGHT / BFS_TILE_HEIGHT;

	srand(time(NULL));
	for (int i = 0; i < count; i++)
	{

		// left Border = 0 ; top Border = 1 ; right Border = 2 ; bottom Border = 3
		int border = ((rand() % 4));		   // Werte 0-3

		int tileIndex;
		if (border == 1 || border == 3)
		{
			tileIndex = ((rand() % TileCountX));  // Werte 0-24
		}
		else
		{
			tileIndex = ((rand() % TileCountY));  // Werte 0-24
		}

		/*
		Enemy* enemy = availableEnemies.back();
		availableEnemies.pop_back();
		_availableEnemiesSize--;
		*/
		Enemy* enemy = getFirstAvailable();
		enemy->visible = true;

		if(enemy){
			switch (border)
			{
			case 0: // left
				enemy->init(0, BFS_TILE_HEIGHT * tileIndex);
				break;
			case 1: // top
				enemy->init(BFS_TILE_WIDTH * tileIndex, 0);
				break;
			case 2: // right
				enemy->init(BFS_TILE_WIDTH * (TileCountX - 1), BFS_TILE_HEIGHT * tileIndex);
				break;
			case 3: // bottom
				enemy->init(BFS_TILE_WIDTH * tileIndex, BFS_TILE_HEIGHT * (TileCountY - 1));
				break;
			}
			enemy->GetSprite()->setCollider(&_collide_enemy);
			_board.attach(enemy->GetSprite());
			//enemies.push_back(enemy);
		}
		g_game.play(g_rc.get_sound("bee.spawn"));
	}
}

void Enemypool::reset(){
	bool done = false;
	//enemies.clear();
	//availableEnemies.clear();
	for(auto & e : enemies)
	{
		done = true;
		e->disappear();
	}

	_spawnCount = 3;
	_spawnTimer = 0;//_spawnTime;
		
	DEBUG_MSG("Enemypool.reset(): Caught." << done);
}

Enemypool::~Enemypool()
{
	for (auto & e : enemies)
	{
		delete e;
	}
	for (auto & a : animations) {
		delete a;
	}
}

void Enemypool::initAnimations() {

	/* Enemy animation definition */

	animations[0] = new Animation(g_rc.get_texture("spritesheet"), 1);
	animations[0]->add_frame(Z_PlaneMeta{ .u = 32 * 0, .v = 32 * 6, .uw = 32, .vw = 32 });
	animations[0]->add_xsheet_phase(0, 1);
	g_rc.add_anim("bug.top", animations[0]);

	animations[1] = new Animation(g_rc.get_texture("spritesheet"), 1);
	animations[1]->add_frame(Z_PlaneMeta{ .u = 32 * 1, .v = 32 * 6, .uw = 32, .vw = 32 });
	animations[1]->add_xsheet_phase(0, 1);
	g_rc.add_anim("bug.topleft", animations[1]);

	animations[2] = new Animation(g_rc.get_texture("spritesheet"), 1);
	animations[2]->add_frame(Z_PlaneMeta{ .u = 32 * 2, .v = 32 * 6, .uw = 32, .vw = 32 });
	animations[2]->add_xsheet_phase(0, 1);
	g_rc.add_anim("bug.left", animations[2]);

	animations[3] = new Animation(g_rc.get_texture("spritesheet"), 1);
	animations[3]->add_frame(Z_PlaneMeta{ .u = 32 * 3, .v = 32 * 6, .uw = 32, .vw = 32 });
	animations[3]->add_xsheet_phase(0, 1);
	g_rc.add_anim("bug.bottomleft", animations[3]);

	animations[4] = new Animation(g_rc.get_texture("spritesheet"), 1);
	animations[4]->add_frame(Z_PlaneMeta{ .u = 32 * 4, .v = 32 * 6, .uw = 32, .vw = 32 });
	animations[4]->add_xsheet_phase(0, 1);
	g_rc.add_anim("bug.bottom", animations[4]);

	animations[5] = new Animation(g_rc.get_texture("spritesheet"), 1);
	animations[5]->add_frame(Z_PlaneMeta{ .u = 32 * 5, .v = 32 * 6, .uw = 32, .vw = 32 });
	animations[5]->add_xsheet_phase(0, 1);
	g_rc.add_anim("bug.bottomright", animations[5]);

	animations[6] = new Animation(g_rc.get_texture("spritesheet"), 1);
	animations[6]->add_frame(Z_PlaneMeta{ .u = 32 * 6, .v = 32 * 6, .uw = 32, .vw = 32 });
	animations[6]->add_xsheet_phase(0, 1);
	g_rc.add_anim("bug.right", animations[6]);

	animations[7] = new Animation(g_rc.get_texture("spritesheet"), 1);
	animations[7]->add_frame(Z_PlaneMeta{ .u = 32 * 7, .v = 32 * 6, .uw = 32, .vw = 32 });
	animations[7]->add_xsheet_phase(0, 1);
	g_rc.add_anim("bug.topright", animations[7]);

	animations[8] = new Animation(g_rc.get_texture("spritesheet"), 1);
	animations[8]->add_frame(Z_PlaneMeta{ .u = 32 * 6, .v = 32 * 5, .uw = 32, .vw = 32 });
	animations[8]->add_xsheet_phase(0, 1);
	g_rc.add_anim("enemy.dead", animations[8]);




	// BEE animations

	animations[9] = new Animation(g_rc.get_texture("spritesheet"), 16);
	animations[9]->add_frame(Z_PlaneMeta{ .pivot_x = 16, .pivot_y = 16, .deg = 0, .u = 32 * 0, .v = 32 * 14, .uw = 32, .vw = 32 });
	animations[9]->add_frame(Z_PlaneMeta{ .pivot_x = 16, .pivot_y = 16, .deg = 0, .u = 32 * 1, .v = 32 * 14, .uw = 32, .vw = 32 });
	animations[9]->add_xsheet_phase(0, 1);
	animations[9]->add_xsheet_phase(1, 1);
	g_rc.add_anim("bee.top", animations[9]);

	animations[10] = new Animation(g_rc.get_texture("spritesheet"), 16);
	animations[10]->add_frame(Z_PlaneMeta{ .pivot_x = 16, .pivot_y = 16, .deg = -45, .u = 32 * 0, .v = 32 * 14, .uw = 32, .vw = 32 });
	animations[10]->add_frame(Z_PlaneMeta{ .pivot_x = 16, .pivot_y = 16, .deg = -45, .u = 32 * 1, .v = 32 * 14, .uw = 32, .vw = 32 });
	animations[10]->add_xsheet_phase(0, 1);
	animations[10]->add_xsheet_phase(1, 1);
	g_rc.add_anim("bee.topleft", animations[10]);

	animations[11] = new Animation(g_rc.get_texture("spritesheet"), 16);
	animations[11]->add_frame(Z_PlaneMeta{ .pivot_x = 16, .pivot_y = 16, .deg = -90, .u = 32 * 0, .v = 32 * 14, .uw = 32, .vw = 32 });
	animations[11]->add_frame(Z_PlaneMeta{ .pivot_x = 16, .pivot_y = 16, .deg = -90, .u = 32 * 1, .v = 32 * 14, .uw = 32, .vw = 32 });
	animations[11]->add_xsheet_phase(0, 1);
	animations[11]->add_xsheet_phase(1, 1);
	g_rc.add_anim("bee.left", animations[11]);

	animations[12] = new Animation(g_rc.get_texture("spritesheet"), 16);
	animations[12]->add_frame(Z_PlaneMeta{ .pivot_x = 16, .pivot_y = 16, .deg = -135, .u = 32 * 0, .v = 32 * 14, .uw = 32, .vw = 32 });
	animations[12]->add_frame(Z_PlaneMeta{ .pivot_x = 16, .pivot_y = 16, .deg = -135, .u = 32 * 1, .v = 32 * 14, .uw = 32, .vw = 32 });
	animations[12]->add_xsheet_phase(0, 1);
	animations[12]->add_xsheet_phase(1, 1);
	g_rc.add_anim("bee.bottomleft", animations[12]);

	animations[13] = new Animation(g_rc.get_texture("spritesheet"), 16);
	animations[13]->add_frame(Z_PlaneMeta{ .pivot_x = 16, .pivot_y = 16, .deg = -180, .u = 32 * 0, .v = 32 * 14, .uw = 32, .vw = 32 });
	animations[13]->add_frame(Z_PlaneMeta{ .pivot_x = 16, .pivot_y = 16, .deg = -180, .u = 32 * 1, .v = 32 * 14, .uw = 32, .vw = 32 });
	animations[13]->add_xsheet_phase(0, 1);
	animations[13]->add_xsheet_phase(1, 1);
	g_rc.add_anim("bee.bottom", animations[13]);

	animations[14] = new Animation(g_rc.get_texture("spritesheet"), 16);
	animations[14]->add_frame(Z_PlaneMeta{ .pivot_x = 16, .pivot_y = 16, .deg = 135, .u = 32 * 0, .v = 32 * 14, .uw = 32, .vw = 32 });
	animations[14]->add_frame(Z_PlaneMeta{ .pivot_x = 16, .pivot_y = 16, .deg = 135, .u = 32 * 1, .v = 32 * 14, .uw = 32, .vw = 32 });
	animations[14]->add_xsheet_phase(0, 1);
	animations[14]->add_xsheet_phase(1, 1);
	g_rc.add_anim("bee.bottomright", animations[14]);

	animations[15] = new Animation(g_rc.get_texture("spritesheet"), 16);
	animations[15]->add_frame(Z_PlaneMeta{ .pivot_x = 16, .pivot_y = 16, .deg = 90, .u = 32 * 0, .v = 32 * 14, .uw = 32, .vw = 32 });
	animations[15]->add_frame(Z_PlaneMeta{ .pivot_x = 16, .pivot_y = 16, .deg = 90, .u = 32 * 1, .v = 32 * 14, .uw = 32, .vw = 32 });
	animations[15]->add_xsheet_phase(0, 1);
	animations[15]->add_xsheet_phase(1, 1);
	g_rc.add_anim("bee.right", animations[15]);

	animations[16] = new Animation(g_rc.get_texture("spritesheet"), 16);
	animations[16]->add_frame(Z_PlaneMeta{ .pivot_x = 16, .pivot_y = 16, .deg = 45, .u = 32 * 0, .v = 32 * 14, .uw = 32, .vw = 32 });
	animations[16]->add_frame(Z_PlaneMeta{ .pivot_x = 16, .pivot_y = 16, .deg = 45, .u = 32 * 1, .v = 32 * 14, .uw = 32, .vw = 32 });
	animations[16]->add_xsheet_phase(0, 1);
	animations[16]->add_xsheet_phase(1, 1);
	g_rc.add_anim("bee.topright", animations[16]);







	// Mealworm animations

	animations[17] = new Animation(g_rc.get_texture("spritesheet"), 4);
	animations[17]->add_frame(Z_PlaneMeta{ .pivot_x = 16, .pivot_y = 16, .deg = 0, .u = 32 * 0, .v = 32 * 13, .uw = 32, .vw = 32 });
	animations[17]->add_frame(Z_PlaneMeta{ .pivot_x = 16, .pivot_y = 16, .deg = 0, .u = 32 * 1, .v = 32 * 13, .uw = 32, .vw = 32 });
	animations[17]->add_frame(Z_PlaneMeta{ .pivot_x = 16, .pivot_y = 16, .deg = 0, .u = 32 * 2, .v = 32 * 13, .uw = 32, .vw = 32 });
	animations[17]->add_xsheet_phase(0, 1);
	animations[17]->add_xsheet_phase(1, 1);
	animations[17]->add_xsheet_phase(2, 1);
	animations[17]->add_xsheet_phase(1, 1);
	g_rc.add_anim("mealworm.top", animations[17]);

	animations[18] = new Animation(g_rc.get_texture("spritesheet"), 4);
	animations[18]->add_frame(Z_PlaneMeta{ .pivot_x = 16, .pivot_y = 16, .deg = -45, .u = 32 * 0, .v = 32 * 13, .uw = 32, .vw = 32 });
	animations[18]->add_frame(Z_PlaneMeta{ .pivot_x = 16, .pivot_y = 16, .deg = -45, .u = 32 * 1, .v = 32 * 13, .uw = 32, .vw = 32 });
	animations[18]->add_frame(Z_PlaneMeta{ .pivot_x = 16, .pivot_y = 16, .deg = -45, .u = 32 * 2, .v = 32 * 13, .uw = 32, .vw = 32 });
	animations[18]->add_xsheet_phase(0, 1);
	animations[18]->add_xsheet_phase(1, 1);
	animations[18]->add_xsheet_phase(2, 1);
	animations[18]->add_xsheet_phase(1, 1);
	g_rc.add_anim("mealworm.topleft", animations[18]);

	animations[19] = new Animation(g_rc.get_texture("spritesheet"), 4);
	animations[19]->add_frame(Z_PlaneMeta{ .pivot_x = 16, .pivot_y = 16, .deg = -90, .u = 32 * 0, .v = 32 * 13, .uw = 32, .vw = 32 });
	animations[19]->add_frame(Z_PlaneMeta{ .pivot_x = 16, .pivot_y = 16, .deg = -90, .u = 32 * 1, .v = 32 * 13, .uw = 32, .vw = 32 });
	animations[19]->add_frame(Z_PlaneMeta{ .pivot_x = 16, .pivot_y = 16, .deg = -90, .u = 32 * 2, .v = 32 * 13, .uw = 32, .vw = 32 });
	animations[19]->add_xsheet_phase(0, 1);
	animations[19]->add_xsheet_phase(1, 1);
	animations[19]->add_xsheet_phase(2, 1);
	animations[19]->add_xsheet_phase(1, 1);
	g_rc.add_anim("mealworm.left", animations[19]);

	animations[20] = new Animation(g_rc.get_texture("spritesheet"), 4);
	animations[20]->add_frame(Z_PlaneMeta{ .pivot_x = 16, .pivot_y = 16, .deg = -135, .u = 32 * 0, .v = 32 * 13, .uw = 32, .vw = 32 });
	animations[20]->add_frame(Z_PlaneMeta{ .pivot_x = 16, .pivot_y = 16, .deg = -135, .u = 32 * 1, .v = 32 * 13, .uw = 32, .vw = 32 });
	animations[20]->add_frame(Z_PlaneMeta{ .pivot_x = 16, .pivot_y = 16, .deg = -135, .u = 32 * 2, .v = 32 * 13, .uw = 32, .vw = 32 });
	animations[20]->add_xsheet_phase(0, 1);
	animations[20]->add_xsheet_phase(1, 1);
	animations[20]->add_xsheet_phase(2, 1);
	animations[20]->add_xsheet_phase(1, 1);
	g_rc.add_anim("mealworm.bottomleft", animations[20]);

	animations[21] = new Animation(g_rc.get_texture("spritesheet"), 4);
	animations[21]->add_frame(Z_PlaneMeta{ .pivot_x = 16, .pivot_y = 16, .deg = -180, .u = 32 * 0, .v = 32 * 13, .uw = 32, .vw = 32 });
	animations[21]->add_frame(Z_PlaneMeta{ .pivot_x = 16, .pivot_y = 16, .deg = -180, .u = 32 * 1, .v = 32 * 13, .uw = 32, .vw = 32 });
	animations[21]->add_frame(Z_PlaneMeta{ .pivot_x = 16, .pivot_y = 16, .deg = -180, .u = 32 * 2, .v = 32 * 13, .uw = 32, .vw = 32 });
	animations[21]->add_xsheet_phase(0, 1);
	animations[21]->add_xsheet_phase(1, 1);
	animations[21]->add_xsheet_phase(2, 1);
	animations[21]->add_xsheet_phase(1, 1);
	g_rc.add_anim("mealworm.bottom", animations[21]);

	animations[22] = new Animation(g_rc.get_texture("spritesheet"), 4);
	animations[22]->add_frame(Z_PlaneMeta{ .pivot_x = 16, .pivot_y = 16, .deg = 135, .u = 32 * 0, .v = 32 * 13, .uw = 32, .vw = 32 });
	animations[22]->add_frame(Z_PlaneMeta{ .pivot_x = 16, .pivot_y = 16, .deg = 135, .u = 32 * 1, .v = 32 * 13, .uw = 32, .vw = 32 });
	animations[22]->add_frame(Z_PlaneMeta{ .pivot_x = 16, .pivot_y = 16, .deg = 135, .u = 32 * 2, .v = 32 * 13, .uw = 32, .vw = 32 });
	animations[22]->add_xsheet_phase(0, 1);
	animations[22]->add_xsheet_phase(1, 1);
	animations[22]->add_xsheet_phase(2, 1);
	animations[22]->add_xsheet_phase(1, 1);
	g_rc.add_anim("mealworm.bottomright", animations[22]);

	animations[23] = new Animation(g_rc.get_texture("spritesheet"), 4);
	animations[23]->add_frame(Z_PlaneMeta{ .pivot_x = 16, .pivot_y = 16, .deg = 90, .u = 32 * 0, .v = 32 * 13, .uw = 32, .vw = 32 });
	animations[23]->add_frame(Z_PlaneMeta{ .pivot_x = 16, .pivot_y = 16, .deg = 90, .u = 32 * 1, .v = 32 * 13, .uw = 32, .vw = 32 });
	animations[23]->add_frame(Z_PlaneMeta{ .pivot_x = 16, .pivot_y = 16, .deg = 90, .u = 32 * 2, .v = 32 * 13, .uw = 32, .vw = 32 });
	animations[23]->add_xsheet_phase(0, 1);
	animations[23]->add_xsheet_phase(1, 1);
	animations[23]->add_xsheet_phase(2, 1);
	animations[23]->add_xsheet_phase(1, 1);
	g_rc.add_anim("mealworm.right", animations[23]);

	animations[24] = new Animation(g_rc.get_texture("spritesheet"), 4);
	animations[24]->add_frame(Z_PlaneMeta{ .pivot_x = 16, .pivot_y = 16, .deg = 45, .u = 32 * 0, .v = 32 * 13, .uw = 32, .vw = 32 });
	animations[24]->add_frame(Z_PlaneMeta{ .pivot_x = 16, .pivot_y = 16, .deg = 45, .u = 32 * 1, .v = 32 * 13, .uw = 32, .vw = 32 });
	animations[24]->add_frame(Z_PlaneMeta{ .pivot_x = 16, .pivot_y = 16, .deg = 45, .u = 32 * 2, .v = 32 * 13, .uw = 32, .vw = 32 });
	animations[24]->add_xsheet_phase(0, 1);
	animations[24]->add_xsheet_phase(1, 1);
	animations[24]->add_xsheet_phase(2, 1);
	animations[24]->add_xsheet_phase(1, 1);
	g_rc.add_anim("mealworm.topright", animations[24]);
}

void Enemypool::handleEvents(Event* e){
	if(e->get("type") == "game.state.set"){
		DEBUG_MSG("Enemypool.handleEvents(e): Caught. game.state.set -> " << e->get("scene"));
		if(e->get("scene") == "gameover"){
			// Do some gamover stuff
		}
		if(e->get("scene") == "game"){
			// Reset everything
		}
	}
};
