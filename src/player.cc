#include "player.hh"
#include "animation.hh"

Player::Player(ResourceManager& rc) : rm(rc) {

	/* Player animation definition */

	animations[0] = new Animation(rc.get_texture("spritesheet"), 5);
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
	rc.add_anim("player.up", animations[0]);

	animations[1] = new Animation(rc.get_texture("spritesheet"), 5);
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
	rc.add_anim("player.down", animations[1]);

	animations[2] = new Animation(rc.get_texture("spritesheet"), 5);
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
	rc.add_anim("player.left", animations[2]);

	animations[3] = new Animation(rc.get_texture("spritesheet"), 5);
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
	rc.add_anim("player.right", animations[3]);
	
	rc.add_sprite("player", &sprite);
	sprite.add_animation("up", rc.get_anim("player.up"));
	sprite.add_animation("down", rc.get_anim("player.down"));
	sprite.add_animation("left", rc.get_anim("player.left"));
	sprite.add_animation("right", rc.get_anim("player.right"));
	sprite.switch_to_anim("left");
}

Player::~Player() {
	for (int i = 0; i < 4; i++) {
		delete animations[i];
	}
}
