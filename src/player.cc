#include "player.hh"

Player::Player(float x, float y, ResourceManager &rc, EventBus &eb, KeyMapper& keymap) : _rc(rc), _eb(eb), _keymap(keymap) {
	playerCoordinates.x = x;
	playerCoordinates.y = y;
	initAnimations();
	initControls();
}

void Player::initAnimations() {

	/* Player animation definition */

	animations[0] = new Animation(_rc.get_texture("spritesheet"), 5);
	animations[0]->add_frame(Z_PlaneMeta{ .u = 0, .v = 32 * 1, .uw = 32, .vw = 32 });
	animations[0]->add_frame(Z_PlaneMeta{ .u = 32 * 1, .v = 32 * 1, .uw = 32, .vw = 32 });
	animations[0]->add_frame(Z_PlaneMeta{ .u = 32 * 2, .v = 32 * 1, .uw = 32, .vw = 32 });
	animations[0]->add_frame(Z_PlaneMeta{ .u = 32 * 3, .v = 32 * 1, .uw = 32, .vw = 32 });
	animations[0]->add_frame(Z_PlaneMeta{ .u = 32 * 4, .v = 32 * 1, .uw = 32, .vw = 32 });
	animations[0]->add_xsheet_phase(0, 1);
	animations[0]->add_xsheet_phase(1, 1);
	animations[0]->add_xsheet_phase(2, 1);
	animations[0]->add_xsheet_phase(3, 1);
	animations[0]->add_xsheet_phase(4, 1);
	animations[0]->add_xsheet_phase(3, 1);
	animations[0]->add_xsheet_phase(2, 1);
	animations[0]->add_xsheet_phase(1, 1);
	_rc.add_anim("player.up", animations[0]);

	animations[1] = new Animation(_rc.get_texture("spritesheet"), 5);
	animations[1]->add_frame(Z_PlaneMeta{ .u = 0, .v = 32 * 0, .uw = 32, .vw = 32 });
	animations[1]->add_frame(Z_PlaneMeta{ .u = 32 * 1, .v = 32 * 0, .uw = 32, .vw = 32 });
	animations[1]->add_frame(Z_PlaneMeta{ .u = 32 * 2, .v = 32 * 0, .uw = 32, .vw = 32 });
	animations[1]->add_frame(Z_PlaneMeta{ .u = 32 * 3, .v = 32 * 0, .uw = 32, .vw = 32 });
	animations[1]->add_frame(Z_PlaneMeta{ .u = 32 * 4, .v = 32 * 0, .uw = 32, .vw = 32 });
	animations[1]->add_xsheet_phase(0, 1);
	animations[1]->add_xsheet_phase(1, 1);
	animations[1]->add_xsheet_phase(2, 1);
	animations[1]->add_xsheet_phase(3, 1);
	animations[1]->add_xsheet_phase(4, 1);
	animations[1]->add_xsheet_phase(3, 1);
	animations[1]->add_xsheet_phase(2, 1);
	animations[1]->add_xsheet_phase(1, 1);
	_rc.add_anim("player.down", animations[1]);

	animations[2] = new Animation(_rc.get_texture("spritesheet"), 5);
	animations[2]->add_frame(Z_PlaneMeta{ .u = 0, .v = 32 * 2, .uw = 32, .vw = 32 });
	animations[2]->add_frame(Z_PlaneMeta{ .u = 32 * 1, .v = 32 * 2, .uw = 32, .vw = 32 });
	animations[2]->add_frame(Z_PlaneMeta{ .u = 32 * 2, .v = 32 * 2, .uw = 32, .vw = 32 });
	animations[2]->add_frame(Z_PlaneMeta{ .u = 32 * 3, .v = 32 * 2, .uw = 32, .vw = 32 });
	animations[2]->add_frame(Z_PlaneMeta{ .u = 32 * 4, .v = 32 * 2, .uw = 32, .vw = 32 });
	animations[2]->add_xsheet_phase(0, 1);
	animations[2]->add_xsheet_phase(1, 1);
	animations[2]->add_xsheet_phase(2, 1);
	animations[2]->add_xsheet_phase(3, 1);
	animations[2]->add_xsheet_phase(4, 1);
	animations[2]->add_xsheet_phase(3, 1);
	animations[2]->add_xsheet_phase(2, 1);
	animations[2]->add_xsheet_phase(1, 1);
	_rc.add_anim("player.left", animations[2]);

	animations[3] = new Animation(_rc.get_texture("spritesheet"), 5);
	animations[3]->add_frame(Z_PlaneMeta{ .u = 0, .v = 32 * 3, .uw = 32, .vw = 32 });
	animations[3]->add_frame(Z_PlaneMeta{ .u = 32 * 1, .v = 32 * 3, .uw = 32, .vw = 32 });
	animations[3]->add_frame(Z_PlaneMeta{ .u = 32 * 2, .v = 32 * 3, .uw = 32, .vw = 32 });
	animations[3]->add_frame(Z_PlaneMeta{ .u = 32 * 3, .v = 32 * 3, .uw = 32, .vw = 32 });
	animations[3]->add_frame(Z_PlaneMeta{ .u = 32 * 4, .v = 32 * 3, .uw = 32, .vw = 32 });
	animations[3]->add_xsheet_phase(0, 1);
	animations[3]->add_xsheet_phase(1, 1);
	animations[3]->add_xsheet_phase(2, 1);
	animations[3]->add_xsheet_phase(3, 1);
	animations[3]->add_xsheet_phase(4, 1);
	animations[3]->add_xsheet_phase(3, 1);
	animations[3]->add_xsheet_phase(2, 1);
	animations[3]->add_xsheet_phase(1, 1);
	_rc.add_anim("player.right", animations[3]);

	_rc.add_sprite("player", &sprite);
	sprite.add_animation("up", _rc.get_anim("player.up"));
	sprite.add_animation("down", _rc.get_anim("player.down"));
	sprite.add_animation("left", _rc.get_anim("player.left"));
	sprite.add_animation("right", _rc.get_anim("player.right"));
	sprite.switch_to_anim("left");
	player = _rc.get_sprite("player");
}

void Player::initControls() {
	f_set_dir = [&](Event* e) {
		auto dir = e->get("direction");
		if (e->get("status_edge") == "down") {
			if (dir == "right") {
				delta.x = 1;
				playerDir = EPlayerDirection::RIGHT;
			};
			if (dir == "up") {
				delta.y = -1;
				playerDir = EPlayerDirection::UP;
			};
			if (dir == "left") {
				delta.x = -1;
				playerDir = EPlayerDirection::LEFT;
			};
			if (dir == "down") {
				delta.y = 1;
				playerDir = EPlayerDirection::DOWN;
			};
		};
		if (e->get("status_edge") == "up") {
			if (dir == "left" || dir == "right") {
				delta.x = 0;
			}
			if (dir == "up" || dir == "down") {
				delta.y = 0;
			}
		}
	};

	// Up
	e_player_up = new Event("player.set_direction");
	e_player_up->set("direction", "up");
	_keymap.bind(SDLK_w, *e_player_up);
	// Down
	e_player_down = new Event("player.set_direction");
	e_player_down->set("direction", "down");
	_keymap.bind(SDLK_s, *e_player_down);
	// Left
	e_player_left = new Event("player.set_direction");
	e_player_left->set("direction", "left");
	_keymap.bind(SDLK_a, *e_player_left);
	// Right
	e_player_right = new Event("player.set_direction");
	e_player_right->set("direction", "right");
	_keymap.bind(SDLK_d, *e_player_right);

	// Register event
	_eb.subscribe("player.set_direction", &f_set_dir);
}

void Player::Update() {

	/* Change player sprite animation*/
	if (pastPlayerDir != playerDir) {
		switch (playerDir) {
		case EPlayerDirection::UP:
			player->switch_to_anim("up");
			break;
		case EPlayerDirection::DOWN:
			player->switch_to_anim("down");
			break;
		case EPlayerDirection::LEFT:
			player->switch_to_anim("left");
			break;
		case EPlayerDirection::RIGHT:
			player->switch_to_anim("right");
			break;
		}
	}

	/* Normalize delta length */
	if (delta.x != 0 || delta.y != 0) {
		const float length_inverse = 1.f / sqrt(delta.x * delta.x + delta.y * delta.y);
		delta.x *= length_inverse * playerSpeed;
		delta.y *= length_inverse * playerSpeed;
	}

	/* Add delta to player coordinates */
	playerCoordinates.x += delta.x;
	playerCoordinates.y += delta.y;

	/* Handle Offscreen Movement */
	if (playerCoordinates.y < 0)				
		playerCoordinates.y -= delta.y;
	if (playerCoordinates.y > (SCREEN_HEIGHT - 64))
		playerCoordinates.y -= delta.y;
	if (playerCoordinates.x < 0)				
		playerCoordinates.x -= delta.x;
	if (playerCoordinates.x > (SCREEN_WIDTH - 64))		
		playerCoordinates.x -= delta.x;

	/* Adjust player sprite transform*/
	player->setTransform(Z_PlaneMeta{ .x = playerCoordinates.x, .y = playerCoordinates.y, .w = 64, .h = 64 });


	pastPlayerDir = playerDir;
}

Sprite* Player::GetSprite() {
	return player;
}

Player::~Player() {
	for (int i = 0; i < 4; i++) {
		delete animations[i];
	}
	delete e_player_up;
	delete e_player_down;
	delete e_player_left;
	delete e_player_right;
}
