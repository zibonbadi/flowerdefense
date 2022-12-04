#include "player.hh"

Player::Player(float x, float y) {
	playerCoordinates.x = x;
	playerCoordinates.y = y;
	initAnimations();

	attack = new Sprite(Z_PlaneMeta{
		.w = 32.0f,
		.h = 32.0f
	});
	attack->set_color(Z_RGBA{.r = 0xff,.g = 0,.b = 0,.a = 0xff});
	attack->setCollider(atk_collide);

	initControls();
	
	// Hook handleEvents into event handler
	this->f_eHandler = new EBus_Fn( std::bind(&Player::handleEvents, this, std::placeholders::_1) );
	// Specify event subscriptions
	//g_eventbus.subscribe("game.state.set", f_eHandler);
	g_eventbus.subscribe("player.die", f_eHandler);
	g_eventbus.subscribe("player.set_direction", f_eHandler);
}

void Player::initAnimations() {

	/* Player animation definition */

	animations[0] = new Animation(g_rc.get_texture("spritesheet"), 5);
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
	g_rc.add_anim("player.up", animations[0]);

	animations[4] = new Animation(g_rc.get_texture("spritesheet"), 5);
	animations[4]->add_frame(Z_PlaneMeta{ .u = 0, .v = 32 * 1, .uw = 32, .vw = 32 });
	animations[4]->add_frame(Z_PlaneMeta{ .u = 32 * 1, .v = 32 * 1, .uw = 32, .vw = 32 });
	animations[4]->add_frame(Z_PlaneMeta{ .u = 32 * 2, .v = 32 * 1, .uw = 32, .vw = 32 });
	animations[4]->add_frame(Z_PlaneMeta{ .u = 32 * 3, .v = 32 * 1, .uw = 32, .vw = 32 });
	animations[4]->add_frame(Z_PlaneMeta{ .u = 32 * 4, .v = 32 * 1, .uw = 32, .vw = 32 });
	animations[4]->add_frame(Z_PlaneMeta{ .u = 32 * 5, .v = 32 * 1, .uw = 32, .vw = 32 });
	animations[4]->add_xsheet_phase(0, 1);
	animations[4]->add_xsheet_phase(5, 1);
	animations[4]->add_xsheet_phase(2, 1);
	animations[4]->add_xsheet_phase(5, 1);
	animations[4]->add_xsheet_phase(4, 1);
	animations[4]->add_xsheet_phase(5, 1);
	animations[4]->add_xsheet_phase(2, 1);
	animations[4]->add_xsheet_phase(5, 1);
	g_rc.add_anim("player.up.damage", animations[4]);

	animations[1] = new Animation(g_rc.get_texture("spritesheet"), 5);
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
	g_rc.add_anim("player.down", animations[1]);

	animations[5] = new Animation(g_rc.get_texture("spritesheet"), 5);
	animations[5]->add_frame(Z_PlaneMeta{ .u = 0, .v = 32 * 0, .uw = 32, .vw = 32 });
	animations[5]->add_frame(Z_PlaneMeta{ .u = 32 * 1, .v = 32 * 0, .uw = 32, .vw = 32 });
	animations[5]->add_frame(Z_PlaneMeta{ .u = 32 * 2, .v = 32 * 0, .uw = 32, .vw = 32 });
	animations[5]->add_frame(Z_PlaneMeta{ .u = 32 * 3, .v = 32 * 0, .uw = 32, .vw = 32 });
	animations[5]->add_frame(Z_PlaneMeta{ .u = 32 * 4, .v = 32 * 0, .uw = 32, .vw = 32 });
	animations[5]->add_frame(Z_PlaneMeta{ .u = 32 * 5, .v = 32 * 0, .uw = 32, .vw = 32 });
	animations[5]->add_xsheet_phase(0, 1);
	animations[5]->add_xsheet_phase(5, 1);
	animations[5]->add_xsheet_phase(2, 1);
	animations[5]->add_xsheet_phase(5, 1);
	animations[5]->add_xsheet_phase(4, 1);
	animations[5]->add_xsheet_phase(5, 1);
	animations[5]->add_xsheet_phase(2, 1);
	animations[5]->add_xsheet_phase(5, 1);
	g_rc.add_anim("player.down.damage", animations[5]);

	animations[2] = new Animation(g_rc.get_texture("spritesheet"), 5);
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
	g_rc.add_anim("player.left", animations[2]);

	animations[6] = new Animation(g_rc.get_texture("spritesheet"), 5);
	animations[6]->add_frame(Z_PlaneMeta{ .u = 0, .v = 32 * 2, .uw = 32, .vw = 32 });
	animations[6]->add_frame(Z_PlaneMeta{ .u = 32 * 1, .v = 32 * 2, .uw = 32, .vw = 32 });
	animations[6]->add_frame(Z_PlaneMeta{ .u = 32 * 2, .v = 32 * 2, .uw = 32, .vw = 32 });
	animations[6]->add_frame(Z_PlaneMeta{ .u = 32 * 3, .v = 32 * 2, .uw = 32, .vw = 32 });
	animations[6]->add_frame(Z_PlaneMeta{ .u = 32 * 4, .v = 32 * 2, .uw = 32, .vw = 32 });
	animations[6]->add_frame(Z_PlaneMeta{ .u = 32 * 5, .v = 32 * 2, .uw = 32, .vw = 32 });
	animations[6]->add_xsheet_phase(0, 1);
	animations[6]->add_xsheet_phase(5, 1);
	animations[6]->add_xsheet_phase(2, 1);
	animations[6]->add_xsheet_phase(5, 1);
	animations[6]->add_xsheet_phase(4, 1);
	animations[6]->add_xsheet_phase(5, 1);
	animations[6]->add_xsheet_phase(2, 1);
	animations[6]->add_xsheet_phase(5, 1);
	g_rc.add_anim("player.left.damage", animations[6]);

	animations[3] = new Animation(g_rc.get_texture("spritesheet"), 5);
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
	g_rc.add_anim("player.right", animations[3]);

	animations[7] = new Animation(g_rc.get_texture("spritesheet"), 5);
	animations[7]->add_frame(Z_PlaneMeta{ .u = 0, .v = 32 * 3, .uw = 32, .vw = 32 });
	animations[7]->add_frame(Z_PlaneMeta{ .u = 32 * 1, .v = 32 * 3, .uw = 32, .vw = 32 });
	animations[7]->add_frame(Z_PlaneMeta{ .u = 32 * 2, .v = 32 * 3, .uw = 32, .vw = 32 });
	animations[7]->add_frame(Z_PlaneMeta{ .u = 32 * 3, .v = 32 * 3, .uw = 32, .vw = 32 });
	animations[7]->add_frame(Z_PlaneMeta{ .u = 32 * 4, .v = 32 * 3, .uw = 32, .vw = 32 });
	animations[7]->add_frame(Z_PlaneMeta{ .u = 32 * 5, .v = 32 * 3, .uw = 32, .vw = 32 });
	animations[7]->add_xsheet_phase(0, 1);
	animations[7]->add_xsheet_phase(5, 1);
	animations[7]->add_xsheet_phase(2, 1);
	animations[7]->add_xsheet_phase(5, 1);
	animations[7]->add_xsheet_phase(4, 1);
	animations[7]->add_xsheet_phase(5, 1);
	animations[7]->add_xsheet_phase(2, 1);
	animations[7]->add_xsheet_phase(5, 1);
	g_rc.add_anim("player.right.damage", animations[7]);

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
}

void Player::initControls() {
	// Up
	e_player_up = new Event("player.set_direction");
	e_player_up->set("direction", "up");
	g_keymapper.bind(SDLK_w, e_player_up);
	// Down
	e_player_down = new Event("player.set_direction");
	e_player_down->set("direction", "down");
	g_keymapper.bind(SDLK_s, e_player_down);
	// Left
	e_player_left = new Event("player.set_direction");
	e_player_left->set("direction", "left");
	g_keymapper.bind(SDLK_a, e_player_left);
	// Right
	e_player_right = new Event("player.set_direction");
	e_player_right->set("direction", "right");
	g_keymapper.bind(SDLK_d, e_player_right);

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
	if(e->get("type") == "player.set_direction"){
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
	switch (playerDir) {
	case EPlayerDirection::UP:
		animID = "up";
		break;
	case EPlayerDirection::DOWN:
		animID = "down";
		break;
	case EPlayerDirection::LEFT:
		animID = "left";
		break;
	case EPlayerDirection::RIGHT:
		animID = "right";
		break;
	}
	_animID = animID + animIDadditional;
	player->switch_to_anim(_animID);
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
	if (pastPlayerDir != playerDir) {
		ChangePlayerAnimation();
	}

	/* Normalize delta length */
	if (delta.x != 0 || delta.y != 0) {
		const float length_inverse = 1.f / sqrt(delta.x * delta.x + delta.y * delta.y);
		delta.x *= length_inverse * playerSpeed;
		delta.y *= length_inverse * playerSpeed;
	}

	/* Add delta to player coordinates */
	playerCoordinates.x += delta.x * deltaTime;
	playerCoordinates.y += delta.y * deltaTime;

	/* Handle Offscreen Movement */
	if (playerCoordinates.y < 0)				
		playerCoordinates.y -= delta.y;
	if (playerCoordinates.y > (SCREEN_HEIGHT - 64))
		playerCoordinates.y -= delta.y;
	if (playerCoordinates.x < 0)				
		playerCoordinates.x -= delta.x;
	if (playerCoordinates.x > (SCREEN_WIDTH - 64))		
		playerCoordinates.x -= delta.x;

	e_player_place_fence->set("player.x",std::to_string(attack->get_transform().x));
	e_player_place_fence->set("player.y",std::to_string(attack->get_transform().y));

	/* Adjust player sprite transform*/
	player->setTransform(Z_PlaneMeta{ .x = playerCoordinates.x, .y = playerCoordinates.y, .w = 64, .h = 64 });
	Z_PlaneMeta tmp_transform = {.w = 64, .h = 64};
	switch(playerDir){
	case EPlayerDirection::LEFT:{
		tmp_transform.x = playerCoordinates.x-64;
		tmp_transform.y = playerCoordinates.y;
		break;
	}
	case EPlayerDirection::RIGHT:{
		tmp_transform.x = playerCoordinates.x+64;
		tmp_transform.y = playerCoordinates.y;
		break;
	}
	case EPlayerDirection::UP:{
		tmp_transform.x = playerCoordinates.x;
		tmp_transform.y = playerCoordinates.y-64;
		break;
	}
	case EPlayerDirection::DOWN:{
		tmp_transform.x = playerCoordinates.x;
		tmp_transform.y = playerCoordinates.y+64;
		break;
	}
	default:{
		break;
	}
	};
	attack->setTransform(tmp_transform);


	pastPlayerDir = playerDir;
}

void Player::reset(float x, float y){
	DEBUG_MSG("Player.reset(): Caught.");
	health=4;
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
	delete e_player_up;
	delete e_player_down;
	delete e_player_left;
	delete e_player_right;
}
