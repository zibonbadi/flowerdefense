#include "enemypool.hh"
#include <cstdio>      /* srand, rand */
#include <ctime>      /* time */

Enemypool::Enemypool(Plane& board, Z_PlaneMeta& collide_enemy, float spawnTimer, int spawnCount, int poolSize) : _board(board), _collide_enemy(collide_enemy), _spawnTime(spawnTimer), _spawnCount(spawnCount), _poolSize(poolSize)
{
	availableEnemies.resize(poolSize);
	for (int i = 0; i < poolSize; i++)
	{
		availableEnemies.push_back(new Enemy(-100.0f, -100.0f, std::to_string(i)));
	}
	//initAnimations();
}

void Enemypool::Update(const float& deltaTime)
{
	_spawnTimer -= deltaTime;
	if (_spawnTimer < 0) 
	{
		Spawn(_spawnCount);
		_spawnTimer = _spawnTime;
	}
}

void Enemypool::Spawn(int count)
{
	if (availableEnemies.size() == 0)
	{
		std::cerr << "Error in Enemypool: No more availableEnemies left to spawn";
		return;
	}

	constexpr int TileCountX = SCREEN_WIDTH / BFS_TILE_WIDTH;
	constexpr int TileCountY = SCREEN_HEIGHT / BFS_TILE_HEIGHT;

	srand(time(NULL));
	size_t pastEnemySize = enemies.size();
	for (int i = pastEnemySize; i < (pastEnemySize + count); i++)
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

		int lastIndex = availableEnemies.size() - 1;
		Enemy* enemy = availableEnemies.back();
		availableEnemies.pop_back();
		switch (border)
		{
		case 0:
			enemy->init(0, BFS_TILE_HEIGHT * tileIndex, std::to_string(lastIndex));
			break;
		case 1:
			enemy->init(BFS_TILE_WIDTH * tileIndex, 0, std::to_string(lastIndex));
			break;
		case 2:
			enemy->init(BFS_TILE_WIDTH * (TileCountX - 1), BFS_TILE_HEIGHT * tileIndex, std::to_string(lastIndex));
			break;
		case 3:
			enemy->init(BFS_TILE_WIDTH * tileIndex, BFS_TILE_HEIGHT * (TileCountY - 1), std::to_string(lastIndex));
			break;
		}
		enemy->GetSprite()->setCollider(&_collide_enemy);
		_board.attach(enemy->GetSprite());
		enemies.push_back(enemy);
	}
}

Enemypool::~Enemypool()
{
	for (int i = 0; i < enemies.size(); i++)
	{
		delete enemies[i];
	}
}

//void Enemypool::initAnimations() {
//
//	/* Enemy animation definition */
//
//	animations[0] = new Animation(g_rc.get_texture("spritesheet"), 1);
//	animations[0]->add_frame(Z_PlaneMeta{ .u = 32 * 0, .v = 32 * 6, .uw = 32, .vw = 32 });
//	animations[0]->add_xsheet_phase(0, 1);
//	g_rc.add_anim(_id+"enemy.top", animations[0]);
//
//	animations[1] = new Animation(g_rc.get_texture("spritesheet"), 1);
//	animations[1]->add_frame(Z_PlaneMeta{ .u = 32 * 1, .v = 32 * 6, .uw = 32, .vw = 32 });
//	animations[1]->add_xsheet_phase(0, 1);
//	g_rc.add_anim(_id + "enemy.topleft", animations[1]);
//
//	animations[2] = new Animation(g_rc.get_texture("spritesheet"), 1);
//	animations[2]->add_frame(Z_PlaneMeta{ .u = 32 * 2, .v = 32 * 6, .uw = 32, .vw = 32 });
//	animations[2]->add_xsheet_phase(0, 1);
//	g_rc.add_anim(_id + "enemy.left", animations[2]);
//
//	animations[3] = new Animation(g_rc.get_texture("spritesheet"), 1);
//	animations[3]->add_frame(Z_PlaneMeta{ .u = 32 * 3, .v = 32 * 6, .uw = 32, .vw = 32 });
//	animations[3]->add_xsheet_phase(0, 1);
//	g_rc.add_anim(_id + "enemy.bottomleft", animations[3]);
//
//	animations[4] = new Animation(g_rc.get_texture("spritesheet"), 1);
//	animations[4]->add_frame(Z_PlaneMeta{ .u = 32 * 4, .v = 32 * 6, .uw = 32, .vw = 32 });
//	animations[4]->add_xsheet_phase(0, 1);
//	g_rc.add_anim(_id + "enemy.bottom", animations[4]);
//
//	animations[5] = new Animation(g_rc.get_texture("spritesheet"), 1);
//	animations[5]->add_frame(Z_PlaneMeta{ .u = 32 * 5, .v = 32 * 6, .uw = 32, .vw = 32 });
//	animations[5]->add_xsheet_phase(0, 1);
//	g_rc.add_anim(_id + "enemy.bottomright", animations[5]);
//
//	animations[6] = new Animation(g_rc.get_texture("spritesheet"), 1);
//	animations[6]->add_frame(Z_PlaneMeta{ .u = 32 * 6, .v = 32 * 6, .uw = 32, .vw = 32 });
//	animations[6]->add_xsheet_phase(0, 1);
//	g_rc.add_anim(_id + "enemy.right", animations[6]);
//
//	animations[7] = new Animation(g_rc.get_texture("spritesheet"), 1);
//	animations[7]->add_frame(Z_PlaneMeta{ .u = 32 * 7, .v = 32 * 6, .uw = 32, .vw = 32 });
//	animations[7]->add_xsheet_phase(0, 1);
//	g_rc.add_anim(_id + "enemy.topright", animations[7]);
//
//	animations[8] = new Animation(g_rc.get_texture("spritesheet"), 1);
//	animations[8]->add_frame(Z_PlaneMeta{ .u = 32 * 6, .v = 32 * 5, .uw = 32, .vw = 32 });
//	animations[8]->add_xsheet_phase(0, 1);
//	g_rc.add_anim(_id+"enemy.dead", animations[8]);
//
//}
//
//Enemypool::~Enemypool() {
//	for (int i = 0; i < 4; i++) {
//		delete animations[i];
//	}
//}
