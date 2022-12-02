#include "hud.hh"

Hud::Hud(Plane &board) : _board(board){
			ex_rahmen_create();
			ex_bar_create();
			gaertner_leben_create();
			rose_leben_create();

}

void Hud::ex_rahmen_create(){
			
			auto xp_rahmen_links = g_rc.make_static_sprite_from_texture("tiles.xp_rahmen_links", "spritesheet", Z_PlaneMeta{ .u = 32 * 0, .v = 32 * 5, .uw = 32, .vw = 32 }).second;
			auto xp_rahmen_rechts = g_rc.make_static_sprite_from_texture("tiles.xp_rahmen_rechts", "spritesheet", Z_PlaneMeta{ .u = 32 * 2, .v = 32 * 5, .uw = 32, .vw = 32 }).second;
			auto xp_rahmen_mitte = g_rc.make_static_sprite_from_texture("tiles.xp_rahmen_mitte", "spritesheet", Z_PlaneMeta{ .u = 32 * 1, .v = 32 * 5, .uw = 32, .vw = 32 }).second;

			ex_rahmen =  new Tilemap(32,32);

			ex_rahmen->add_tile('-', xp_rahmen_links);
			ex_rahmen->add_tile('/', xp_rahmen_rechts);
			ex_rahmen->add_tile('+', xp_rahmen_mitte);


			ex_rahmen->fill(0, 0, 1, 1, '-');
			ex_rahmen->fill(24, 0, 1, 1, '/');
			ex_rahmen->fill(0, 0, 24, 1, '+');

			_board.attach(ex_rahmen);

}

void Hud::ex_bar_create(){
			 ex_balken_anzeige = new Animation(g_rc.get_texture("spritesheet"), 1);
			ex_balken_anzeige->add_frame(Z_PlaneMeta{ .u = (32*3)-3, .v = 32 * 5, .uw = 32, .vw = 32 });
			ex_balken_anzeige->add_xsheet_phase(0, 1);
			g_rc.add_anim("ex_balken.ex_balken_anzeige", ex_balken_anzeige);
			 ex_bar = g_rc.make_sprite_from_anim("ex_balken", "ex_balken.ex_balken_anzeige", Z_PlaneMeta { .x = 0, .y = 0, .w = 0, .h = 0 }).second;

		_board.attach(ex_bar);
}

void Hud::gaertner_leben_create(){

			gaertner_leben_1 = new Animation(g_rc.get_texture("spritesheet"), 1);
		

			gaertner_leben_1->add_frame(Z_PlaneMeta{ .u = 32 * 5, .v = 32 * 4, .uw = 32, .vw = 32 });



			gaertner_leben_1->add_xsheet_phase(0, 1);

			
			g_rc.add_anim("gaertner_leben.gaertner_leben_1", gaertner_leben_1);


			 gaertner_leben[0] = g_rc.make_sprite_from_anim("gaertner_leben", "gaertner_leben.gaertner_leben_1", Z_PlaneMeta { .x = 0, .y = 32, .w = 32, .h = 32 }).second;
			 gaertner_leben[1] = g_rc.make_sprite_from_anim("gaertner_leben", "gaertner_leben.gaertner_leben_1", Z_PlaneMeta { .x = 32 * 1, .y = 32, .w = 32, .h = 32 }).second;
			 gaertner_leben[2] = g_rc.make_sprite_from_anim("gaertner_leben", "gaertner_leben.gaertner_leben_1", Z_PlaneMeta { .x = 32 * 2, .y = 32, .w = 32, .h = 32 }).second;
			 gaertner_leben[3] = g_rc.make_sprite_from_anim("gaertner_leben", "gaertner_leben.gaertner_leben_1", Z_PlaneMeta { .x = 32 * 3, .y = 32, .w = 32, .h = 32 }).second;


			 _board.attach(gaertner_leben[0]);
			 _board.attach(gaertner_leben[1]);
			 _board.attach(gaertner_leben[2]);
			 _board.attach(gaertner_leben[3]);
}

void Hud::rose_leben_create(){
			 rose_leben_a[0] = new Animation(g_rc.get_texture("spritesheet"), 1);
			 rose_leben_a[1] = new Animation(g_rc.get_texture("spritesheet"), 1);
			 rose_leben_a[2] = new Animation(g_rc.get_texture("spritesheet"), 1);
			 rose_leben_a[3] = new Animation(g_rc.get_texture("spritesheet"), 1);

		

			rose_leben_a[3]->add_frame(Z_PlaneMeta{ .u = 32 * 0, .v = 32 * 4, .uw = 32, .vw = 32 });
			rose_leben_a[2]->add_frame(Z_PlaneMeta{ .u = 32 * 1, .v = 32 * 4, .uw = 32, .vw = 32 });
			rose_leben_a[1]->add_frame(Z_PlaneMeta{ .u = 32 * 2, .v = 32 * 4, .uw = 32, .vw = 32 });
			rose_leben_a[0]->add_frame(Z_PlaneMeta{ .u = 32 * 3, .v = 32 * 4, .uw = 32, .vw = 32 });



			rose_leben_a[3]->add_xsheet_phase(0, 1);
			rose_leben_a[2]->add_xsheet_phase(0, 1);
			rose_leben_a[1]->add_xsheet_phase(0, 1);
			rose_leben_a[0]->add_xsheet_phase(0, 1);


		

			g_rc.add_anim("rose_leben.rose_leben_4", rose_leben_a[3]);
			g_rc.add_anim("rose_leben.rose_leben_3", rose_leben_a[2]);
			g_rc.add_anim("rose_leben.rose_leben_2", rose_leben_a[1]);
			g_rc.add_anim("rose_leben.rose_leben_1", rose_leben_a[0]);

			 rose_leben[3] = g_rc.make_sprite_from_anim("rose_leben", "rose_leben.rose_leben_4", Z_PlaneMeta { .x = 32*23, .y = 32, .w = 64, .h = 64}).second;
			 rose_leben[2] = g_rc.make_sprite_from_anim("rose_leben", "rose_leben.rose_leben_3", Z_PlaneMeta { }).second;
			 rose_leben[1] = g_rc.make_sprite_from_anim("rose_leben", "rose_leben.rose_leben_2", Z_PlaneMeta {  }).second;
			 rose_leben[0] = g_rc.make_sprite_from_anim("rose_leben", "rose_leben.rose_leben_1", Z_PlaneMeta { }).second;
			
			_board.attach(rose_leben[0]);
			_board.attach(rose_leben[1]);
			_board.attach(rose_leben[2]);
			_board.attach(rose_leben[3]);

}

void Hud::rose_leben_runter(){
	switch (rose_akt_leben)
	{
	case 4:
		rose_leben[3]->setTransform(Z_PlaneMeta{});	
		rose_leben[2]->setTransform(Z_PlaneMeta{.x = 32*23, .y = 32, .w = 64, .h = 64});
		rose_akt_leben--;
		return;
	case 3:
		rose_leben[2]->setTransform(Z_PlaneMeta{});	
		rose_leben[1]->setTransform(Z_PlaneMeta{.x = 32*23, .y = 32, .w = 64, .h = 64});
		rose_akt_leben--;
		return;
	case 2:
		rose_leben[1]->setTransform(Z_PlaneMeta{});	
		rose_leben[0]->setTransform(Z_PlaneMeta{.x = 32*23, .y = 32, .w = 64, .h = 64});
		rose_akt_leben--;
		return;
	case 1:
		//game over
		break;
	
	default:
		break;
	}
}

void Hud::rose_leben_hoch(){
	switch (rose_akt_leben)
	{
	case 1:
		rose_leben[0]->setTransform(Z_PlaneMeta{});	
		rose_leben[1]->setTransform(Z_PlaneMeta{.x = 32*23, .y = 32, .w = 64, .h = 64});
		rose_akt_leben++;
		return;
	case 2:
		rose_leben[1]->setTransform(Z_PlaneMeta{});	
		rose_leben[2]->setTransform(Z_PlaneMeta{.x = 32*23, .y = 32, .w = 64, .h = 64});
		rose_akt_leben++;
		return;
	case 3:
		rose_leben[2]->setTransform(Z_PlaneMeta{});	
		rose_leben[3]->setTransform(Z_PlaneMeta{.x = 32*23, .y = 32, .w = 64, .h = 64});
		rose_akt_leben++;
		return;
	
	default:
		break;
	}


	// rose_akt_leben--;


}

void Hud::gaertner_leben_runter(){
	switch (gaetner_akt_leben)
	{
	case 4:
		gaertner_leben[3]->setTransform(Z_PlaneMeta{});	
		gaetner_akt_leben--;
		return;
	case 3:
		gaertner_leben[2]->setTransform(Z_PlaneMeta{});	
		gaetner_akt_leben--;
		return;
	case 2:
		gaertner_leben[1]->setTransform(Z_PlaneMeta{});	
		gaetner_akt_leben--;
		return;
	case 1:
		//game over
		break;
	
	default:
		break;
	}
}

void Hud::gaertner_leben_hoch(){
	switch (gaetner_akt_leben)
	{
	case 1:
		gaertner_leben[1]->setTransform(Z_PlaneMeta{.x = 32 * 1, .y = 32, .w = 32, .h = 32 });

		gaetner_akt_leben++;
		return;
	case 2:
		gaertner_leben[2]->setTransform(Z_PlaneMeta{.x = 32 * 2, .y = 32, .w = 32, .h = 32 });
		gaetner_akt_leben++;
		return;
	case 3:
		gaertner_leben[3]->setTransform(Z_PlaneMeta{.x = 32 * 3, .y = 32, .w = 32, .h = 32 });
		gaetner_akt_leben++;
		return;
	
	default:
		break;
	}
}

void Hud::ex_bar_steuern(float ex){
		ex_bar->setTransform(Z_PlaneMeta{ .x = -200, .y = 0, .w = ex, .h = 32 });

}

void Hud::exp_create(float x, float y){
 			ex_anzeige = new Animation(g_rc.get_texture("spritesheet"), 1);
			ex_anzeige->add_frame(Z_PlaneMeta{ .u = 32 * 6, .v = 32 * 5, .uw = 32, .vw = 32  });
			ex_anzeige->add_xsheet_phase(0, 1);
			g_rc.add_anim("ex.ex_anzeige", ex_anzeige);
			 ex = g_rc.make_sprite_from_anim("ex_anzeige", "ex.ex_anzeige", Z_PlaneMeta { .x = 32 * x, .y = 32 * y, .w = 32, .h = 32 }).second;

		_board.attach(ex);

}
