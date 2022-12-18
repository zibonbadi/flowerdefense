#include "enemy.hh"

Enemy::Enemy(float x, float y, Player& player, EEnemyType enemyType, EEnemyDirection enemyDir) : _player(player), _enemyType(enemyType), _enemyDir(enemyDir) {
	init(x, y);
	setSpriteAnimations();
}

void Enemy::init(float x, float y)
{
	coordinates.x = x;
	coordinates.y = y;
	goalTileCoordinates.x = x;
	goalTileCoordinates.y = y;
	sprite.debug_sprite = _player.GetSprite()->debug_sprite;
	sprite.debug_collide = _player.GetSprite()->debug_collide;

	switch (_enemyType)
	{
	case EEnemyType::BUG:
		_enemySpeed = 1.5f;
		break;
	case EEnemyType::MEALWORM:
		_enemySpeed = 0.5f;
		break;
	case EEnemyType::BEE:
		_enemySpeed = 2.5f;
		break;
	default:
		break;
	}
}

void Enemy::setSpriteAnimations() {
	//g_rc.add_sprite("enemy", &sprite);
	std::string enemy_type_id;

	switch (_enemyType)
	{
	case EEnemyType::BUG:
		enemy_type_id = "bug";

		break;
	case EEnemyType::MEALWORM:
		enemy_type_id = "mealworm";
		// TODO
		break;
	case EEnemyType::BEE:
		enemy_type_id = "bee";
		break;
	default:
		break;
	}
	sprite.add_animation("top", g_rc.get_anim(enemy_type_id + ".top"));
	sprite.add_animation("bottom", g_rc.get_anim(enemy_type_id + ".bottom"));
	sprite.add_animation("left", g_rc.get_anim(enemy_type_id + ".left"));
	sprite.add_animation("right", g_rc.get_anim(enemy_type_id + ".right"));
	sprite.add_animation("topleft", g_rc.get_anim(enemy_type_id + ".topleft"));
	sprite.add_animation("topright", g_rc.get_anim(enemy_type_id + ".topright"));
	sprite.add_animation("bottomleft", g_rc.get_anim(enemy_type_id + ".bottomleft"));
	sprite.add_animation("bottomright", g_rc.get_anim(enemy_type_id + ".bottomright"));
	sprite.add_animation("dead", g_rc.get_anim("enemy.dead"));
	sprite.switch_to_anim("right");
}

void Enemy::Update(const BFS& bfsFlower, const BFS& bfsPlayer) {
	if (!this->visible) {
		return;
	}

	SDL_FPoint flightPathRose = {
	.x = (SCREEN_WIDTH / 2) - coordinates.x,
	.y = (SCREEN_HEIGHT / 2) - coordinates.y,
	};

	float flightPathRoseLength = sqrt(flightPathRose.x * flightPathRose.x + flightPathRose.y * flightPathRose.y);



	Tilemap* bfsArrows;

	if (_enemyType == EEnemyType::BUG) {
		
		SDL_FPoint flightPathPlayer = {
			.x = _player.playerCoordinates.x - coordinates.x,
			.y = _player.playerCoordinates.y - coordinates.y,
		};
		float flightPathPlayerLength = sqrt(flightPathPlayer.x * flightPathPlayer.x + flightPathPlayer.y * flightPathPlayer.y);

		if (flightPathPlayerLength < flightPathRoseLength) {
			bfsArrows = bfsPlayer.bfsArrows;
		}
		else {
			bfsArrows = bfsFlower.bfsArrows;
		}
	}

	if(!isdead){
		int tileSize = -1;
		char movingDir = -1;
		const char reverseBFSDir[] = { 'b','t','r','l','4','1','2', '3' };

		if (_enemyType == EEnemyType::BUG) {
			tileSize = bfsArrows->get_transform().w;
			movingDir = bfsArrows->get_spot(coordinates.x / tileSize, coordinates.y / tileSize);
		}

		if (_enemyType == EEnemyType::MEALWORM) {
			movingDir = reverseBFSDir[(int)_enemyDir];
		}

		if (_enemyType == EEnemyType::BEE) {
			float angle = atan2(flightPathRose.y, flightPathRose.x) * 180 / M_PI;
			angle -= 90.f;
			DEBUG_MSG(angle)
			if (angle < -67.5 && angle >= -112.5) {
				movingDir = reverseBFSDir[(int)EEnemyDirection::RIGHT];
			}
			else if (angle < -112.5 && angle >= -157.5) {
				movingDir = reverseBFSDir[(int)EEnemyDirection::TOPRIGHT];
			}
			else if (angle < -157.5 || angle > 157.5) {
				movingDir = reverseBFSDir[(int)EEnemyDirection::TOP];
			}
			else if (angle > 112.5 && angle <= 157.5) {
				movingDir = reverseBFSDir[(int)EEnemyDirection::TOPLEFT];
			}
			else if (angle > 67.5 && angle <= 112.5) {
				movingDir = reverseBFSDir[(int)EEnemyDirection::LEFT];
			}
			else if (angle > 22.5 && angle <= 67.5) {
				movingDir = reverseBFSDir[(int)EEnemyDirection::BOTTOMLEFT];
			}
			else if (angle <= 22.5 || angle >= -22.5) {
				movingDir = reverseBFSDir[(int)EEnemyDirection::BOTTOM];
			}
			else if (angle < -22.5 && angle >= -67.5) {
				movingDir = reverseBFSDir[(int)EEnemyDirection::BOTTOMRIGHT];
			}
		}

		/* Change bug sprite animation*/
		if (goalTileCoordinates.x == ((int)(coordinates.x)) &&
			goalTileCoordinates.y == ((int)(coordinates.y))) {
			switch (movingDir) {
			case 't':
				if (_enemyDir != EEnemyDirection::TOP) sprite.switch_to_anim("top");
				_enemyDir = EEnemyDirection::TOP;
				goalTileCoordinates.y -= tileSize;
				break;
			case 'b':
				if (_enemyDir != EEnemyDirection::BOTTOM) sprite.switch_to_anim("bottom");
				_enemyDir = EEnemyDirection::BOTTOM;
				goalTileCoordinates.y += tileSize;
				break;
			case 'l':
				if (_enemyDir != EEnemyDirection::LEFT) sprite.switch_to_anim("left");
				_enemyDir = EEnemyDirection::LEFT;
				goalTileCoordinates.x -= tileSize;
				break;
			case 'r':
				if (_enemyDir != EEnemyDirection::RIGHT) sprite.switch_to_anim("right");
				_enemyDir = EEnemyDirection::RIGHT;
				goalTileCoordinates.x += tileSize;
				break;
			case '1':
				if (_enemyDir != EEnemyDirection::TOPLEFT) sprite.switch_to_anim("topleft");
				_enemyDir = EEnemyDirection::TOPLEFT;
				goalTileCoordinates.x -= tileSize;
				goalTileCoordinates.y -= tileSize;
				break;
			case '2':
				if (_enemyDir != EEnemyDirection::TOPRIGHT) sprite.switch_to_anim("topright");
				_enemyDir = EEnemyDirection::TOPRIGHT;
				goalTileCoordinates.x += tileSize;
				goalTileCoordinates.y -= tileSize;
				break;
			case '3':
				if (_enemyDir != EEnemyDirection::BOTTOMLEFT) sprite.switch_to_anim("bottomleft");
				_enemyDir = EEnemyDirection::BOTTOMLEFT;
				goalTileCoordinates.x -= tileSize;
				goalTileCoordinates.y += tileSize;
				break;
			case '4':
				if (_enemyDir != EEnemyDirection::BOTTOMRIGHT) sprite.switch_to_anim("bottomright");
				_enemyDir = EEnemyDirection::BOTTOMRIGHT;
				goalTileCoordinates.x += tileSize;
				goalTileCoordinates.y += tileSize;
				break;
			}
		}

		interpolStepSize.x = goalTileCoordinates.x - coordinates.x;
		interpolStepSize.y = goalTileCoordinates.y - coordinates.y;

		/* Normalize delta length */
		if (interpolStepSize.x != 0 || interpolStepSize.y != 0) {
			const float length_inverse = 1.f / sqrt(interpolStepSize.x * interpolStepSize.x + interpolStepSize.y * interpolStepSize.y);
			interpolStepSize.x *= length_inverse;
			interpolStepSize.y *= length_inverse;
		}

		SDL_FPoint adjustedInterpolStepSize;
		adjustedInterpolStepSize.x = interpolStepSize.x * _enemySpeed;
		adjustedInterpolStepSize.y = interpolStepSize.y * _enemySpeed;

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
	}
}

Sprite* Enemy::GetSprite() {
	return &sprite;
}

void Enemy::dying(){
	isdead = true;
	sprite.switch_to_anim("dead");
}

void Enemy::disappear(){
	sprite.setTransform(Z_PlaneMeta{ .x = -INFINITY, .y = -INFINITY });
	visible = false;
}

void Enemy::reborn(float x, float y){
	isdead = false;
	//visible = true;
	init(x, y);
}
