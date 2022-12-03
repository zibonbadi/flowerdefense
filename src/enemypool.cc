#include "enemypool.hh"
#include <cstdio>      /* srand, rand */
#include <ctime>      /* time */

Enemypool::Enemypool(Plane& board, Player& player, Z_PlaneMeta& collide_enemy, float spawnTime, int spawnCount, int poolSize) : _board(board), _player(player), _collide_enemy(collide_enemy), _spawnTime(spawnTime), _spawnCount(spawnCount), _poolSize(poolSize)
{
	availableEnemies.resize(poolSize);
	_availableEnemiesSize = poolSize;

	initAnimations();

	for (int i = 0; i < poolSize; i++)
	{
		availableEnemies.push_back(new Enemy(-100.0f, -100.0f, _player));
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

void Enemypool::Recollect()
{
	for (int i = 0; i < enemies.size(); i++)
	{
		if (!enemies[i]->visible) {

			enemies[i]->reborn(-100.0f, -100.0f);
			availableEnemies.push_back(enemies[i]);
			_availableEnemiesSize++;
			enemies.erase(enemies.begin() + i);
		}
	}
}

void Enemypool::Spawn(int count)
{
	if ((_availableEnemiesSize - count) < 0)
	{
		std::cerr << "Error in Enemypool: Not enough availableEnemies left to spawn" << std::endl;
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
		if (border == 1 || border == 2)
		{
			tileIndex = ((rand() % TileCountX));  // Werte 0-24
		}
		else
		{
			tileIndex = ((rand() % TileCountY));  // Werte 0-24
		}

		Enemy* enemy = availableEnemies.back();
		availableEnemies.pop_back();
		_availableEnemiesSize--;
		switch (border)
		{
		case 0:
			enemy->init(0, BFS_TILE_HEIGHT * tileIndex);
			break;
		case 1:
			enemy->init(BFS_TILE_WIDTH * tileIndex, 0);
			break;
		case 2:
			enemy->init(BFS_TILE_WIDTH * (TileCountX - 1), BFS_TILE_HEIGHT * tileIndex);
			break;
		case 3:
			enemy->init(BFS_TILE_WIDTH * tileIndex, BFS_TILE_HEIGHT * (TileCountY - 1));
			break;
		}
		enemy->GetSprite()->setCollider(&_collide_enemy);
		_board.attach(enemy->GetSprite());
		enemies.push_back(enemy);
	}
}

void Enemypool::reset(){
	bool done = false;
	//enemies.clear();
	//availableEnemies.clear();
	for(; enemies.size() > 0; _availableEnemiesSize++)
	{
		done = true;
		auto en = enemies.back();
		enemies.pop_back();
		availableEnemies.push_back(en);
		//en->init(-100.0f, -100.0f);
		en->disappear();
	}

	DEBUG_MSG("Enemypool.reset(): Caught." << done);
}

Enemypool::~Enemypool()
{
	for (int i = 0; i < enemies.size(); i++)
	{
		delete enemies[i];
	}
	for (int i = 0; i < 9; i++) {
		delete animations[i];
	}
}

void Enemypool::initAnimations() {

	/* Enemy animation definition */

	animations[0] = new Animation(g_rc.get_texture("spritesheet"), 1);
	animations[0]->add_frame(Z_PlaneMeta{ .u = 32 * 0, .v = 32 * 6, .uw = 32, .vw = 32 });
	animations[0]->add_xsheet_phase(0, 1);
	g_rc.add_anim("enemy.top", animations[0]);

	animations[1] = new Animation(g_rc.get_texture("spritesheet"), 1);
	animations[1]->add_frame(Z_PlaneMeta{ .u = 32 * 1, .v = 32 * 6, .uw = 32, .vw = 32 });
	animations[1]->add_xsheet_phase(0, 1);
	g_rc.add_anim("enemy.topleft", animations[1]);

	animations[2] = new Animation(g_rc.get_texture("spritesheet"), 1);
	animations[2]->add_frame(Z_PlaneMeta{ .u = 32 * 2, .v = 32 * 6, .uw = 32, .vw = 32 });
	animations[2]->add_xsheet_phase(0, 1);
	g_rc.add_anim("enemy.left", animations[2]);

	animations[3] = new Animation(g_rc.get_texture("spritesheet"), 1);
	animations[3]->add_frame(Z_PlaneMeta{ .u = 32 * 3, .v = 32 * 6, .uw = 32, .vw = 32 });
	animations[3]->add_xsheet_phase(0, 1);
	g_rc.add_anim("enemy.bottomleft", animations[3]);

	animations[4] = new Animation(g_rc.get_texture("spritesheet"), 1);
	animations[4]->add_frame(Z_PlaneMeta{ .u = 32 * 4, .v = 32 * 6, .uw = 32, .vw = 32 });
	animations[4]->add_xsheet_phase(0, 1);
	g_rc.add_anim("enemy.bottom", animations[4]);

	animations[5] = new Animation(g_rc.get_texture("spritesheet"), 1);
	animations[5]->add_frame(Z_PlaneMeta{ .u = 32 * 5, .v = 32 * 6, .uw = 32, .vw = 32 });
	animations[5]->add_xsheet_phase(0, 1);
	g_rc.add_anim("enemy.bottomright", animations[5]);

	animations[6] = new Animation(g_rc.get_texture("spritesheet"), 1);
	animations[6]->add_frame(Z_PlaneMeta{ .u = 32 * 6, .v = 32 * 6, .uw = 32, .vw = 32 });
	animations[6]->add_xsheet_phase(0, 1);
	g_rc.add_anim("enemy.right", animations[6]);

	animations[7] = new Animation(g_rc.get_texture("spritesheet"), 1);
	animations[7]->add_frame(Z_PlaneMeta{ .u = 32 * 7, .v = 32 * 6, .uw = 32, .vw = 32 });
	animations[7]->add_xsheet_phase(0, 1);
	g_rc.add_anim("enemy.topright", animations[7]);

	animations[8] = new Animation(g_rc.get_texture("spritesheet"), 1);
	animations[8]->add_frame(Z_PlaneMeta{ .u = 32 * 6, .v = 32 * 5, .uw = 32, .vw = 32 });
	animations[8]->add_xsheet_phase(0, 1);
	g_rc.add_anim("enemy.dead", animations[8]);
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
