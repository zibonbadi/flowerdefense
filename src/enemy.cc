#include "enemy.hh"

Enemy::Enemy(float x, float y, ResourceManager &rc, EventBus &eb, std::string id) : _rc(rc), _eb(eb), _id(id) {
	coordinates.x = x;
	coordinates.y = y;
	goalTileCoordinates.x = x;
	goalTileCoordinates.y = y;
	initAnimations();
}

void Enemy::initAnimations() {

	/* Enemy animation definition */

	animations[0] = new Animation(_rc.get_texture("spritesheet"), 1);
	animations[0]->add_frame(Z_PlaneMeta{ .u = 32 * 0, .v = 32 * 6, .uw = 32, .vw = 32 });
	animations[0]->add_xsheet_phase(0, 1);
	_rc.add_anim(_id+"enemy.top", animations[0]);

	animations[1] = new Animation(_rc.get_texture("spritesheet"), 1);
	animations[1]->add_frame(Z_PlaneMeta{ .u = 32 * 1, .v = 32 * 6, .uw = 32, .vw = 32 });
	animations[1]->add_xsheet_phase(0, 1);
	_rc.add_anim(_id + "enemy.topleft", animations[1]);

	animations[2] = new Animation(_rc.get_texture("spritesheet"), 1);
	animations[2]->add_frame(Z_PlaneMeta{ .u = 32 * 2, .v = 32 * 6, .uw = 32, .vw = 32 });
	animations[2]->add_xsheet_phase(0, 1);
	_rc.add_anim(_id + "enemy.left", animations[2]);

	animations[3] = new Animation(_rc.get_texture("spritesheet"), 1);
	animations[3]->add_frame(Z_PlaneMeta{ .u = 32 * 3, .v = 32 * 6, .uw = 32, .vw = 32 });
	animations[3]->add_xsheet_phase(0, 1);
	_rc.add_anim(_id + "enemy.bottomleft", animations[3]);

	animations[4] = new Animation(_rc.get_texture("spritesheet"), 1);
	animations[4]->add_frame(Z_PlaneMeta{ .u = 32 * 4, .v = 32 * 6, .uw = 32, .vw = 32 });
	animations[4]->add_xsheet_phase(0, 1);
	_rc.add_anim(_id + "enemy.bottom", animations[4]);

	animations[5] = new Animation(_rc.get_texture("spritesheet"), 1);
	animations[5]->add_frame(Z_PlaneMeta{ .u = 32 * 5, .v = 32 * 6, .uw = 32, .vw = 32 });
	animations[5]->add_xsheet_phase(0, 1);
	_rc.add_anim(_id + "enemy.bottomright", animations[5]);

	animations[6] = new Animation(_rc.get_texture("spritesheet"), 1);
	animations[6]->add_frame(Z_PlaneMeta{ .u = 32 * 6, .v = 32 * 6, .uw = 32, .vw = 32 });
	animations[6]->add_xsheet_phase(0, 1);
	_rc.add_anim(_id + "enemy.right", animations[6]);

	animations[7] = new Animation(_rc.get_texture("spritesheet"), 1);
	animations[7]->add_frame(Z_PlaneMeta{ .u = 32 * 7, .v = 32 * 6, .uw = 32, .vw = 32 });
	animations[7]->add_xsheet_phase(0, 1);
	_rc.add_anim(_id + "enemy.topright", animations[7]);

	_rc.add_sprite(_id + "enemy", &sprite);
	sprite.add_animation("top",		_rc.get_anim(_id + "enemy.top"));
	sprite.add_animation("bottom",	_rc.get_anim(_id + "enemy.bottom"));
	sprite.add_animation("left",	_rc.get_anim(_id + "enemy.left"));
	sprite.add_animation("right",	_rc.get_anim(_id + "enemy.right"));
	sprite.add_animation("topleft", _rc.get_anim(_id + "enemy.topleft"));
	sprite.add_animation("topright", _rc.get_anim(_id + "enemy.topright"));
	sprite.add_animation("bottomleft", _rc.get_anim(_id + "enemy.bottomleft"));
	sprite.add_animation("bottomright", _rc.get_anim(_id + "enemy.bottomright"));
	sprite.switch_to_anim("right");
	enemy = _rc.get_sprite(_id + "enemy");
}

void Enemy::Update(Tilemap* bfsArrows) {
	if(!isdead){
	const int tileSize = bfsArrows->get_transform().w;
	char movingDir = bfsArrows->get_spot(coordinates.x / tileSize, coordinates.y / tileSize);

	/* Change enemy sprite animation*/
	if (goalTileCoordinates.x == ((int)(coordinates.x)) &&
		goalTileCoordinates.y == ((int)(coordinates.y))) {
		switch (movingDir) {
			case 't':
				enemyDir = EEnemyDirection::TOP;
				sprite.switch_to_anim("top");
				goalTileCoordinates.y -= tileSize;
				break;
			case 'b':
				enemyDir = EEnemyDirection::BOTTOM;
				sprite.switch_to_anim("bottom");
				goalTileCoordinates.y += tileSize;
				break;
			case 'l':
				enemyDir = EEnemyDirection::LEFT;
				sprite.switch_to_anim("left");
				goalTileCoordinates.x -= tileSize;
				break;
			case 'r':
				enemyDir = EEnemyDirection::RIGHT;
				sprite.switch_to_anim("right");
				goalTileCoordinates.x += tileSize;
				break;
			case '1':
				enemyDir = EEnemyDirection::TOPLEFT;
				sprite.switch_to_anim("topleft");
				goalTileCoordinates.x -= tileSize;
				goalTileCoordinates.y -= tileSize;
				break;
			case '2':
				enemyDir = EEnemyDirection::TOPRIGHT;
				sprite.switch_to_anim("topright");
				goalTileCoordinates.x += tileSize;
				goalTileCoordinates.y -= tileSize;
				break;
			case '3':
				enemyDir = EEnemyDirection::BOTTOMLEFT;
				sprite.switch_to_anim("bottomleft");
				goalTileCoordinates.x -= tileSize;
				goalTileCoordinates.y += tileSize;
				break;
			case '4':
				enemyDir = EEnemyDirection::BOTTOMRIGHT;
				sprite.switch_to_anim("bottomright");
				goalTileCoordinates.x += tileSize;
				goalTileCoordinates.y += tileSize;
				break;
		}

		interpolStepSize.x = goalTileCoordinates.x - coordinates.x;
		interpolStepSize.y = goalTileCoordinates.y - coordinates.y;

		/* Normalize delta length */
		if (interpolStepSize.x != 0 || interpolStepSize.y != 0) {
			const float length_inverse = 1.f / sqrt(interpolStepSize.x * interpolStepSize.x + interpolStepSize.y * interpolStepSize.y);
			interpolStepSize.x *= length_inverse;
			interpolStepSize.y *= length_inverse;
		}

	}

	SDL_FPoint adjustedInterpolStepSize;
	adjustedInterpolStepSize.x = interpolStepSize.x * enemySpeed;
	adjustedInterpolStepSize.y = interpolStepSize.y * enemySpeed;

	if (abs(goalTileCoordinates.x - coordinates.x) < abs(adjustedInterpolStepSize.x)) {
		adjustedInterpolStepSize.x = goalTileCoordinates.x - coordinates.x;
	}

	if (abs(goalTileCoordinates.y - coordinates.y) < abs(adjustedInterpolStepSize.y)) {
		adjustedInterpolStepSize.y = goalTileCoordinates.y - coordinates.y;
	}

	/* Add delta to enemy coordinates */
	coordinates.x += adjustedInterpolStepSize.x;
	coordinates.y += adjustedInterpolStepSize.y;

	/* Adjust enemy sprite transform*/             // x - 8 so that the sprite centroid moves exactly along the bfs path
	enemy->setTransform(Z_PlaneMeta{ .x = coordinates.x - 8, .y = coordinates.y - 8, .w = 32, .h = 32 });
}}

Sprite* Enemy::GetSprite() {
	return enemy;
}

void Enemy::dying(){
			animations[0] = new Animation(_rc.get_texture("spritesheet"), 1);
			animations[0]->add_frame(Z_PlaneMeta{ .u = 32 * 6, .v = 32 * 5, .uw = 32, .vw = 32 });
			animations[0]->add_xsheet_phase(0, 1);
			_rc.add_anim(_id+"enemy.dead", animations[0]);
			sprite.add_animation("dead", _rc.get_anim(_id + "enemy.dead"));
			sprite.switch_to_anim("dead");
			enemy = _rc.get_sprite(_id + "enemy");

		isdead = true;
}

void Enemy::disappear(){
			enemy->setTransform(Z_PlaneMeta{  });

}


Enemy::~Enemy() {
	for (int i = 0; i < 4; i++) {
		delete animations[i];
	}
}
