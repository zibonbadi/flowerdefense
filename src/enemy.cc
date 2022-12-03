#include "enemy.hh"

Enemy::Enemy(float x, float y) {
	init(x, y);
	setSpriteAnimations();
}

void Enemy::init(float x, float y)
{
	coordinates.x = x;
	coordinates.y = y;
	goalTileCoordinates.x = x;
	goalTileCoordinates.y = y;
}

void Enemy::setSpriteAnimations() {
	g_rc.add_sprite("enemy", &sprite);
	sprite.add_animation("top",		g_rc.get_anim("enemy.top"));
	sprite.add_animation("bottom",	g_rc.get_anim("enemy.bottom"));
	sprite.add_animation("left",	g_rc.get_anim("enemy.left"));
	sprite.add_animation("right",	g_rc.get_anim("enemy.right"));
	sprite.add_animation("topleft", g_rc.get_anim("enemy.topleft"));
	sprite.add_animation("topright", g_rc.get_anim("enemy.topright"));
	sprite.add_animation("bottomleft", g_rc.get_anim("enemy.bottomleft"));
	sprite.add_animation("bottomright", g_rc.get_anim("enemy.bottomright"));
	sprite.add_animation("dead", g_rc.get_anim("enemy.dead"));
	sprite.switch_to_anim("right");
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
	sprite.setTransform(Z_PlaneMeta{ .x = coordinates.x - 8, .y = coordinates.y - 8, .w = 32, .h = 32 });
}}

Sprite* Enemy::GetSprite() {
	return &sprite;
}

void Enemy::dying(){
	isdead = true;
	sprite.switch_to_anim("dead");
}

void Enemy::disappear(){
	sprite.setTransform(Z_PlaneMeta{});

}

void Enemy::reborn(float x, float y){
	isdead = false;

	coordinates.x = x;
	coordinates.y = y;
	goalTileCoordinates.x = x;
	goalTileCoordinates.y = y;
}
