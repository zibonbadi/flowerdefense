#include "enemypool.hh"

Enemypool::Enemypool(int count)
{
	//for (int i = 0; i < count; i++)
	//{
	//	enemies.push_back(new Enemy());
	//}
	//enemies.push_back(&enemy2);
	//enemies.push_back(&enemy3);
	initAnimations();
}

void Enemypool::initAnimations() {

	/* Enemy animation definition */

	animations[0] = new Animation(g_rc.get_texture("spritesheet"), 1);
	animations[0]->add_frame(Z_PlaneMeta{ .u = 32 * 0, .v = 32 * 6, .uw = 32, .vw = 32 });
	animations[0]->add_xsheet_phase(0, 1);
	g_rc.add_anim(_id+"enemy.top", animations[0]);

	animations[1] = new Animation(g_rc.get_texture("spritesheet"), 1);
	animations[1]->add_frame(Z_PlaneMeta{ .u = 32 * 1, .v = 32 * 6, .uw = 32, .vw = 32 });
	animations[1]->add_xsheet_phase(0, 1);
	g_rc.add_anim(_id + "enemy.topleft", animations[1]);

	animations[2] = new Animation(g_rc.get_texture("spritesheet"), 1);
	animations[2]->add_frame(Z_PlaneMeta{ .u = 32 * 2, .v = 32 * 6, .uw = 32, .vw = 32 });
	animations[2]->add_xsheet_phase(0, 1);
	g_rc.add_anim(_id + "enemy.left", animations[2]);

	animations[3] = new Animation(g_rc.get_texture("spritesheet"), 1);
	animations[3]->add_frame(Z_PlaneMeta{ .u = 32 * 3, .v = 32 * 6, .uw = 32, .vw = 32 });
	animations[3]->add_xsheet_phase(0, 1);
	g_rc.add_anim(_id + "enemy.bottomleft", animations[3]);

	animations[4] = new Animation(g_rc.get_texture("spritesheet"), 1);
	animations[4]->add_frame(Z_PlaneMeta{ .u = 32 * 4, .v = 32 * 6, .uw = 32, .vw = 32 });
	animations[4]->add_xsheet_phase(0, 1);
	g_rc.add_anim(_id + "enemy.bottom", animations[4]);

	animations[5] = new Animation(g_rc.get_texture("spritesheet"), 1);
	animations[5]->add_frame(Z_PlaneMeta{ .u = 32 * 5, .v = 32 * 6, .uw = 32, .vw = 32 });
	animations[5]->add_xsheet_phase(0, 1);
	g_rc.add_anim(_id + "enemy.bottomright", animations[5]);

	animations[6] = new Animation(g_rc.get_texture("spritesheet"), 1);
	animations[6]->add_frame(Z_PlaneMeta{ .u = 32 * 6, .v = 32 * 6, .uw = 32, .vw = 32 });
	animations[6]->add_xsheet_phase(0, 1);
	g_rc.add_anim(_id + "enemy.right", animations[6]);

	animations[7] = new Animation(g_rc.get_texture("spritesheet"), 1);
	animations[7]->add_frame(Z_PlaneMeta{ .u = 32 * 7, .v = 32 * 6, .uw = 32, .vw = 32 });
	animations[7]->add_xsheet_phase(0, 1);
	g_rc.add_anim(_id + "enemy.topright", animations[7]);

	animations[8] = new Animation(g_rc.get_texture("spritesheet"), 1);
	animations[8]->add_frame(Z_PlaneMeta{ .u = 32 * 6, .v = 32 * 5, .uw = 32, .vw = 32 });
	animations[8]->add_xsheet_phase(0, 1);
	g_rc.add_anim(_id+"enemy.dead", animations[8]);

	g_rc.add_sprite(_id + "enemy", &sprite);
	sprite.add_animation(_id + "top",		g_rc.get_anim(_id + "enemy.top"));
	sprite.add_animation(_id + "bottom",	g_rc.get_anim(_id + "enemy.bottom"));
	sprite.add_animation(_id + "left",	g_rc.get_anim(_id + "enemy.left"));
	sprite.add_animation(_id + "right",	g_rc.get_anim(_id + "enemy.right"));
	sprite.add_animation(_id + "topleft", g_rc.get_anim(_id + "enemy.topleft"));
	sprite.add_animation(_id + "topright", g_rc.get_anim(_id + "enemy.topright"));
	sprite.add_animation(_id + "bottomleft", g_rc.get_anim(_id + "enemy.bottomleft"));
	sprite.add_animation(_id + "bottomright", g_rc.get_anim(_id + "enemy.bottomright"));
	sprite.add_animation(_id + "dead", g_rc.get_anim(_id + "enemy.dead"));
	sprite.switch_to_anim(_id + "right");
	enemy = g_rc.get_sprite(_id + "enemy");
}

Enemypool::~Enemypool() {
	for (int i = 0; i < 4; i++) {
		delete animations[i];
	}
}
