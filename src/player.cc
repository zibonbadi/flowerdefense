#include "player.hh"

Player::Player(float x, float y) {
	playerCoordinates.x = x;
	playerCoordinates.y = y;
	initAnimations();
	attack->set_color(Z_RGBA{.r = 0xff,.g = 0,.b = 0,.a = 0xff});
	attack->setCollider(atk_collide);

	initControls();
	
	// Hook handleEvents into event handler
	this->f_eHandler = new EBus_Fn( std::bind(&Player::handleEvents, this, std::placeholders::_1) );
	// Specify event subscriptions
	//g_eventbus.subscribe("game.state.set", f_eHandler);
	g_eventbus.subscribe("player.die", f_eHandler);
	g_eventbus.subscribe("player.set_move_direction", f_eHandler);
	g_eventbus.subscribe("player.set_attack_direction", f_eHandler);
}

void Player::initAnimations() {

	/* Player animation definition */

	animations[0] = new Animation(g_rc.get_texture("spritesheet"), 5);
	animations[0]->add_frame(Z_PlaneMeta{ .u = 32 * 1, .v = 0, .uw = 32, .vw = 64 });
	animations[0]->add_xsheet_phase(0, 1);
	g_rc.add_anim("player.up", animations[0]);

	animations[4] = new Animation(g_rc.get_texture("spritesheet"), 5);
	animations[4]->add_frame(Z_PlaneMeta{ .u = 32 * 1, .v = 64 * 1, .uw = 32, .vw = 64 });
	animations[4]->add_xsheet_phase(0, 1);
	g_rc.add_anim("player.up.damage", animations[4]);

	animations[1] = new Animation(g_rc.get_texture("spritesheet"), 5);
	animations[1]->add_frame(Z_PlaneMeta{ .u = 32 * 0, .v = 64 * 0, .uw = 32, .vw = 64 });
	animations[1]->add_xsheet_phase(0, 1);
	g_rc.add_anim("player.down", animations[1]);

	animations[5] = new Animation(g_rc.get_texture("spritesheet"), 5);
	animations[5]->add_frame(Z_PlaneMeta{ .u = 32 * 0, .v = 64 * 1, .uw = 32, .vw = 64 });
	animations[5]->add_xsheet_phase(0, 1);
	g_rc.add_anim("player.down.damage", animations[5]);

	animations[2] = new Animation(g_rc.get_texture("spritesheet"), 5);
	animations[2]->add_frame(Z_PlaneMeta{ .u = 32 * 2, .v = 64 * 0, .uw = 32, .vw = 64 });
	animations[2]->add_xsheet_phase(0, 1);
	g_rc.add_anim("player.left", animations[2]);

	animations[6] = new Animation(g_rc.get_texture("spritesheet"), 5);
	animations[6]->add_frame(Z_PlaneMeta{ .u = 32 * 2, .v = 64 * 1, .uw = 32, .vw = 64 });
	animations[6]->add_xsheet_phase(0, 1);
	g_rc.add_anim("player.left.damage", animations[6]);

	animations[3] = new Animation(g_rc.get_texture("spritesheet"), 5);
	animations[3]->add_frame(Z_PlaneMeta{ .u = 32 * 3, .v = 64 * 0, .uw = 32, .vw = 64 });
	animations[3]->add_xsheet_phase(0, 1);
	g_rc.add_anim("player.right", animations[3]);

	animations[7] = new Animation(g_rc.get_texture("spritesheet"), 5);
	animations[7]->add_frame(Z_PlaneMeta{ .u = 32*3, .v = 64 * 1, .uw = 32, .vw = 64 });
	animations[7]->add_xsheet_phase(0, 1);
	g_rc.add_anim("player.right.damage", animations[7]);

	/* Attack animation definition */
	attack = new Sprite(Z_PlaneMeta{
		.w = 64.0f,
		.h = 64.0f
	});

	animations[10] = new Animation(g_rc.get_texture("spritesheet"), 18);
	animations[10]->add_frame(Z_PlaneMeta{ .u = 32*0, .v = 32 * 9, .uw = 32, .vw = 32 });
	animations[10]->add_frame(Z_PlaneMeta{ .u = 32*1, .v = 32 * 9, .uw = 32, .vw = 32 });
	animations[10]->add_frame(Z_PlaneMeta{ .u = 32*2, .v = 32 * 9, .uw = 32, .vw = 32 });
	animations[10]->add_frame(Z_PlaneMeta{ .u = 32*3, .v = 32 * 9, .uw = 32, .vw = 32 });
	animations[10]->add_frame(Z_PlaneMeta{ .u = 32*4, .v = 32 * 9, .uw = 32, .vw = 32 });
	animations[10]->add_xsheet_phase(0, 1);
	animations[10]->add_xsheet_phase(1, 1);
	animations[10]->add_xsheet_phase(2, 2);
	animations[10]->add_xsheet_phase(3, 2);
	animations[10]->add_xsheet_phase(4, 1);
	animations[10]->add_xsheet_phase(2, 2);
	animations[10]->add_xsheet_phase(4, 1);
	animations[10]->add_xsheet_phase(1, 2);
	animations[10]->add_xsheet_phase(3, 1);
	animations[10]->add_xsheet_phase(4, 1);
	animations[10]->add_xsheet_phase(1, 1);
	animations[10]->add_xsheet_phase(2, 3);
	animations[10]->add_xsheet_phase(3, 2);
	animations[10]->add_xsheet_phase(1, 1);
	g_rc.add_anim("player.attack.left", animations[10]);

	animations[11] = new Animation(g_rc.get_texture("spritesheet"), 18);
	animations[11]->add_frame(Z_PlaneMeta{.pivot_x = 32, .pivot_y = 32, .deg = 180, .u = 32*0, .v = 32 * 9, .uw = 32, .vw = 32 });
	animations[11]->add_frame(Z_PlaneMeta{.pivot_x = 32, .pivot_y = 32, .deg = 180, .u = 32*1, .v = 32 * 9, .uw = 32, .vw = 32 });
	animations[11]->add_frame(Z_PlaneMeta{.pivot_x = 32, .pivot_y = 32, .deg = 180, .u = 32*2, .v = 32 * 9, .uw = 32, .vw = 32 });
	animations[11]->add_frame(Z_PlaneMeta{.pivot_x = 32, .pivot_y = 32, .deg = 180, .u = 32*3, .v = 32 * 9, .uw = 32, .vw = 32 });
	animations[11]->add_frame(Z_PlaneMeta{.pivot_x = 32, .pivot_y = 32, .deg = 180, .u = 32*4, .v = 32 * 9, .uw = 32, .vw = 32 });
	animations[11]->add_xsheet_phase(0, 1);
	animations[11]->add_xsheet_phase(1, 1);
	animations[11]->add_xsheet_phase(2, 2);
	animations[11]->add_xsheet_phase(3, 2);
	animations[11]->add_xsheet_phase(4, 1);
	animations[11]->add_xsheet_phase(2, 2);
	animations[11]->add_xsheet_phase(4, 1);
	animations[11]->add_xsheet_phase(1, 2);
	animations[11]->add_xsheet_phase(3, 1);
	animations[11]->add_xsheet_phase(4, 1);
	animations[11]->add_xsheet_phase(1, 1);
	animations[11]->add_xsheet_phase(2, 3);
	animations[11]->add_xsheet_phase(3, 2);
	animations[11]->add_xsheet_phase(1, 1);
	g_rc.add_anim("player.attack.right", animations[11]);

	animations[8] = new Animation(g_rc.get_texture("spritesheet"), 18);
	animations[8]->add_frame(Z_PlaneMeta{.pivot_x = 32, .pivot_y = 32, .deg = 90,  .u = 32*0, .v = 32 * 9, .uw = 32, .vw = 32 });
	animations[8]->add_frame(Z_PlaneMeta{.pivot_x = 32, .pivot_y = 32, .deg = 90,  .u = 32*1, .v = 32 * 9, .uw = 32, .vw = 32 });
	animations[8]->add_frame(Z_PlaneMeta{.pivot_x = 32, .pivot_y = 32, .deg = 90,  .u = 32*2, .v = 32 * 9, .uw = 32, .vw = 32 });
	animations[8]->add_frame(Z_PlaneMeta{.pivot_x = 32, .pivot_y = 32, .deg = 90,  .u = 32*3, .v = 32 * 9, .uw = 32, .vw = 32 });
	animations[8]->add_frame(Z_PlaneMeta{.pivot_x = 32, .pivot_y = 32, .deg = 90,  .u = 32*4, .v = 32 * 9, .uw = 32, .vw = 32 });
	animations[8]->add_xsheet_phase(0, 1);
	animations[8]->add_xsheet_phase(1, 1);
	animations[8]->add_xsheet_phase(2, 2);
	animations[8]->add_xsheet_phase(3, 2);
	animations[8]->add_xsheet_phase(4, 1);
	animations[8]->add_xsheet_phase(2, 2);
	animations[8]->add_xsheet_phase(4, 1);
	animations[8]->add_xsheet_phase(1, 2);
	animations[8]->add_xsheet_phase(3, 1);
	animations[8]->add_xsheet_phase(4, 1);
	animations[8]->add_xsheet_phase(1, 1);
	animations[8]->add_xsheet_phase(2, 3);
	animations[8]->add_xsheet_phase(3, 2);
	animations[8]->add_xsheet_phase(1, 1);
	g_rc.add_anim("player.attack.up", animations[8]);

	animations[9] = new Animation(g_rc.get_texture("spritesheet"), 18);
	animations[9]->add_frame(Z_PlaneMeta{.pivot_x = 32, .pivot_y = 32, .deg = 270,  .u = 32*0, .v = 32 * 9, .uw = 32, .vw = 32 });
	animations[9]->add_frame(Z_PlaneMeta{.pivot_x = 32, .pivot_y = 32, .deg = 270,  .u = 32*1, .v = 32 * 9, .uw = 32, .vw = 32 });
	animations[9]->add_frame(Z_PlaneMeta{.pivot_x = 32, .pivot_y = 32, .deg = 270,  .u = 32*2, .v = 32 * 9, .uw = 32, .vw = 32 });
	animations[9]->add_frame(Z_PlaneMeta{.pivot_x = 32, .pivot_y = 32, .deg = 270,  .u = 32*3, .v = 32 * 9, .uw = 32, .vw = 32 });
	animations[9]->add_frame(Z_PlaneMeta{.pivot_x = 32, .pivot_y = 32, .deg = 270,  .u = 32*4, .v = 32 * 9, .uw = 32, .vw = 32 });
	animations[9]->add_xsheet_phase(0, 1);
	animations[9]->add_xsheet_phase(1, 1);
	animations[9]->add_xsheet_phase(2, 2);
	animations[9]->add_xsheet_phase(3, 2);
	animations[9]->add_xsheet_phase(4, 1);
	animations[9]->add_xsheet_phase(2, 2);
	animations[9]->add_xsheet_phase(4, 1);
	animations[9]->add_xsheet_phase(1, 2);
	animations[9]->add_xsheet_phase(3, 1);
	animations[9]->add_xsheet_phase(4, 1);
	animations[9]->add_xsheet_phase(1, 1);
	animations[9]->add_xsheet_phase(2, 3);
	animations[9]->add_xsheet_phase(3, 2);
	animations[9]->add_xsheet_phase(1, 1);
	g_rc.add_anim("player.attack.down", animations[9]);

	/* Plugging things together */
	g_rc.add_sprite("player", &sprite);
	sprite.add_animation("up", g_rc.get_anim("player.up"));
	sprite.add_animation("down", g_rc.get_anim("player.down"));
	sprite.add_animation("left", g_rc.get_anim("player.left"));
	sprite.add_animation("right", g_rc.get_anim("player.right"));
	sprite.add_animation("up.damage", g_rc.get_anim("player.up.damage"));
	sprite.add_animation("down.damage", g_rc.get_anim("player.down.damage"));
	sprite.add_animation("left.damage", g_rc.get_anim("player.left.damage"));
	sprite.add_animation("right.damage", g_rc.get_anim("player.right.damage"));
	sprite.switch_to_anim("left");
	player = g_rc.get_sprite("player");
	attack->add_animation("up", g_rc.get_anim("player.attack.up"));
	attack->add_animation("down", g_rc.get_anim("player.attack.down"));
	attack->add_animation("left", g_rc.get_anim("player.attack.left"));
	attack->add_animation("right", g_rc.get_anim("player.attack.right"));
	attack->switch_to_anim("up");
}

void Player::initControls() {
	// Move Up
	e_player_move_up = new Event("player.set_move_direction");
	e_player_move_up->set("direction", "up");
	g_keymapper.bind(SDLK_w, e_player_move_up);
	// Move Down
	e_player_move_down = new Event("player.set_move_direction");
	e_player_move_down->set("direction", "down");
	g_keymapper.bind(SDLK_s, e_player_move_down);
	// Move Left
	e_player_move_left = new Event("player.set_move_direction");
	e_player_move_left->set("direction", "left");
	g_keymapper.bind(SDLK_a, e_player_move_left);
	// Move Right
	e_player_move_right = new Event("player.set_move_direction");
	e_player_move_right->set("direction", "right");
	g_keymapper.bind(SDLK_d, e_player_move_right);

	// Attack Up
	e_player_attack_up = new Event("player.set_attack_direction");
	e_player_attack_up->set("direction", "up");
	g_keymapper.bind(SDLK_UP, e_player_attack_up);
	// Attack Down
	e_player_attack_down = new Event("player.set_attack_direction");
	e_player_attack_down->set("direction", "down");
	g_keymapper.bind(SDLK_DOWN, e_player_attack_down);
	// Attack Left
	e_player_attack_left = new Event("player.set_attack_direction");
	e_player_attack_left->set("direction", "left");
	g_keymapper.bind(SDLK_LEFT, e_player_attack_left);
	// Attack Right
	e_player_attack_right = new Event("player.set_attack_direction");
	e_player_attack_right->set("direction", "right");
	g_keymapper.bind(SDLK_RIGHT, e_player_attack_right);

	// Place Fence
	e_player_place_fence = new Event("player.place_fence");
	e_player_place_fence->set("direction", "up");
	g_keymapper.bind(SDLK_SPACE, e_player_place_fence);
}

void Player::handleEvents(Event* e){
	if(e->get("type") == "player.die"){
		Event e_gameover("game.state.set");
		e_gameover.set("scene", "gameover");
		g_eventbus.send(&e_gameover);
	}
	if (e->get("type") == "player.set_attack_direction") {
		auto dir = e->get("direction");
		if (e->get("status_edge") == "down") {
			if (dir == "right") {
				attackDir = EPlayerAttackDirection::RIGHT;
			};
			if (dir == "up") {
				attackDir = EPlayerAttackDirection::UP;
			};
			if (dir == "left") {
				attackDir = EPlayerAttackDirection::LEFT;
			};
			if (dir == "down") {
				attackDir = EPlayerAttackDirection::DOWN;
			};
		};
	}
	if (e->get("type") == "player.set_move_direction") {
		auto dir = e->get("direction");
		if (e->get("status_edge") == "down") {
			if (dir == "right") {
				walk_right = true;
				playerDir = EPlayerMoveDirection::RIGHT;
			};
			if (dir == "up") {
				walk_up = true;
				playerDir = EPlayerMoveDirection::UP;
			};
			if (dir == "left") {
				walk_left = true;
				playerDir = EPlayerMoveDirection::LEFT;
			};
			if (dir == "down") {
				walk_down = true;
				playerDir = EPlayerMoveDirection::DOWN;
			};
		};
		if (e->get("status_edge") == "up") {
			if (dir == "right") {
				walk_right = false;
			};
			if (dir == "up") {
				walk_up = false;
			};
			if (dir == "left") {
				walk_left = false;
			};
			if (dir == "down") {
				walk_down = false;
			};
		}
		// Non-XOR to reset delta
		if (walk_up == walk_down) {
			delta.y = 0;
		}else{
			delta.y = (walk_up)?-1:1;
		}
		if (walk_left == walk_right) {
			delta.x = 0;
		}else{
			delta.x = (walk_left)?-1:1;
		};
	}
	if(e->get("type") == "game.state.set"){
		DEBUG_MSG("Player.handleEvents(e): Caught. game.state.set -> " << e->get("scene"));
		if(e->get("scene") == "gameover"){
			// Do some gamover stuff
		}
		if(e->get("scene") == "game"){
			// Reset everything
		}
	}
};

void Player::ChangePlayerAnimation(const std::string animIDadditional = "")
{
	std::string animID;
	switch (attackDir) {
	case EPlayerAttackDirection::UP:
		animID = "up";
		break;
	case EPlayerAttackDirection::DOWN:
		animID = "down";
		break;
	case EPlayerAttackDirection::LEFT:
		animID = "left";
		break;
	case EPlayerAttackDirection::RIGHT:
		animID = "right";
		break;
	}
	_animID = animID + animIDadditional;
	DEBUG_MSG("Switched to player anim \"" << _animID << "\";\"" << animID << "\"")
	player->switch_to_anim(_animID);
	attack->switch_to_anim(animID);
}

void Player::Update(const float& deltaTime, const std::vector<Enemy*>& enemies) {

	bool collide_player = false;
	bool collide_attack = false;
	for (Enemy* enemy : enemies) {
		if (this->attack->collision(enemy->GetSprite())) {
			if(!enemy->isdead && enemy->visible){
				enemy->dying();
			}
		}
		if (this->player->collision(enemy->GetSprite())) {
			if(enemy->isdead && enemy->visible){ // falls enemy ein xp pickup
			enemy->disappear();
			xp_bar += xp_bekommt;

			}else if(!enemy->isdead){
			collide_player = true;
			enemy->dying();
			enemy->disappear();
			xp_bar += xp_bekommt;
			}
		}
	}
	if (collide_player) {
		if (_animID.find(".damage") == std::string::npos) {
			ChangePlayerAnimation(".damage");
			health--;
		}
		this->damageAnimCooldown = 0.3;
	}
	else if (this->damageAnimCooldown > 0) {
		this->damageAnimCooldown -= deltaTime;
		if (this->damageAnimCooldown <= 0) {
			ChangePlayerAnimation();

		}
	}

	if(health<1){
		Event die("player.die");
		g_eventbus.send(&die);
	}

	/* Change player sprite animation*/
	if (pastAttackDir != attackDir) {
		ChangePlayerAnimation();
	}

	/* Normalize delta length */
	if (delta.x != 0 || delta.y != 0) {
		const float length_inverse = 1.f / sqrt(delta.x * delta.x + delta.y * delta.y);
		delta.x *= length_inverse * playerSpeed * deltaTime;
		delta.y *= length_inverse * playerSpeed * deltaTime;
	}

	/* Add delta to player coordinates */
	playerCoordinates.x += delta.x;
	playerCoordinates.y += delta.y;

	/* Handle Offscreen Movement */
	if (playerCoordinates.y < 0)				
		playerCoordinates.y = 0;
	if (playerCoordinates.y > (SCREEN_HEIGHT - 64))
		playerCoordinates.y = (SCREEN_HEIGHT - 64);
	if (playerCoordinates.x < 0)				
		playerCoordinates.x = 0;
	if (playerCoordinates.x > (SCREEN_WIDTH - 64))		
		playerCoordinates.x = (SCREEN_WIDTH - 64);

	/* Adjust event values */
	e_player_place_fence->set("player.x",std::to_string(playerCoordinates.x));
	e_player_place_fence->set("player.y",std::to_string(playerCoordinates.y));

	/* Adjust player sprite transform*/
	player->setTransform(Z_PlaneMeta{ .x = playerCoordinates.x, .y = playerCoordinates.y, .w = 64, .h = 64 });
	Z_PlaneMeta tmp_transform = {.w = 64, .h = 64};
	switch(attackDir){
	case EPlayerAttackDirection::LEFT:{
		tmp_transform.x = playerCoordinates.x-64;
		tmp_transform.y = playerCoordinates.y;
		break;
	}
	case EPlayerAttackDirection::RIGHT:{
		tmp_transform.x = playerCoordinates.x+64;
		tmp_transform.y = playerCoordinates.y;
		break;
	}
	case EPlayerAttackDirection::UP:{
		tmp_transform.x = playerCoordinates.x;
		tmp_transform.y = playerCoordinates.y-64;
		break;
	}
	case EPlayerAttackDirection::DOWN:{
		tmp_transform.x = playerCoordinates.x;
		tmp_transform.y = playerCoordinates.y+64;
		break;
	}
	default:{
		break;
	}
	};
	attack->setTransform(tmp_transform);


	pastAttackDir = attackDir;
}

void Player::reset(float x, float y){
	DEBUG_MSG("Player.reset(): Caught.");
	health=4;
	obstacles=0;
	akt_LV=0;
	playerCoordinates.x = x;
	playerCoordinates.y = y;
}

Sprite* Player::GetSprite() {
	return player;
}

Player::~Player() {
	for (int i = 0; i < 4; i++) {
		delete animations[i];
	}
	delete attack;
	delete atk_collide;
	delete f_eHandler;
	delete e_player_move_up;
	delete e_player_move_down;
	delete e_player_move_left;
	delete e_player_move_right;
}
