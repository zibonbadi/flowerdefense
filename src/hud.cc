#include "hud.hh"

Hud::Hud(Plane &board) : _board(board){
			LVLS = new Tilemap(400,200);
			LVLS->transform(Z_PlaneMeta{.x = 200, .y = 0});

			 //auto LVLSy = g_rc.make_static_sprite_from_texture("tiles.plus_eins", "plus_eins", Z_PlaneMeta{ .u = 0, .v = 0, .uw = 4000, .vw = 4000 }).second;

			// LVLS =  new Tilemap(32,32);
			// LVLS->add_tile('?', LVLSy);

			// LVLS->fill(8, 5, 10, 5, '?');

			// _board.attach(LVLS);

			//LVLSY[0] = g_rc.make_static_sprite_from_texture("tiles.lvlUp_option1", "lvlUp_option", Z_PlaneMeta{ .u = 0, .v = 0, .uw = 400, .vw = 200 }).second;
			//LVLSY[1] = g_rc.make_static_sprite_from_texture("tiles.lvlUp_option2", "lvlUp_option", Z_PlaneMeta{ .u = 0 , .v = 200, .uw = 400, .vw = 200 }).second;
			//LVLSY[2] = g_rc.make_static_sprite_from_texture("tiles.lvlUp_option3", "lvlUp_option", Z_PlaneMeta{ .u = 0, .v = 400, .uw = 400, .vw = 200 }).second;

			LVLSY[0] = g_rc.make_static_sprite_from_texture("tiles.lvlUp.option.header", "lvlUp_option", Z_PlaneMeta{ .u = 0, .v = 0, .uw = 400, .vw = 200 }).second;
			LVLSY[1] = g_rc.make_static_sprite_from_texture("tiles.lvlUp.option.speed", "lvlUp_option", Z_PlaneMeta{ .u = 0, .v = 185, .uw = 400, .vw = 200 }).second;
			LVLSY[2] = g_rc.make_static_sprite_from_texture("tiles.lvlUp.option.fences", "lvlUp_option", Z_PlaneMeta{ .u = 0, .v = 400, .uw = 400, .vw = 200 }).second;
			LVLSY[3] = g_rc.make_static_sprite_from_texture("tiles.lvlUp.option.fire", "lvlUp_option", Z_PlaneMeta{ .u = 0, .v = 600, .uw = 400, .vw = 200 }).second;
			LVLSY[4] = g_rc.make_static_sprite_from_texture("tiles.lvlUp.option.health", "lvlUp_option", Z_PlaneMeta{ .u = 0, .v = 800, .uw = 400, .vw = 200 }).second;

			//LVLSY[0]->transform()

			LVLS->add_tile('=', LVLSY[0]);
			LVLS->add_tile('?', LVLSY[1]);
			LVLS->add_tile('/', LVLSY[2]);
			LVLS->add_tile('o', LVLSY[3]);
			LVLS->add_tile('+', LVLSY[4]);

			LVLS->fill(0, 0, 1, 1, '=');
			LVLS->fill(0, 1, 1, 1, '?');
			LVLS->fill(0, 2, 1, 1, '/');
			LVLS->fill(0, 3, 1, 1, '+');

			LVLS->visible = false;
			_board.attach(LVLS);

			ex_rahmen_create();
			ex_bar_create();
			gaertner_leben_create();
			rose_leben_create();
			text_layers_create();
			font_create();
			eBus_setup();
}

Hud::~Hud(){
	_board.detach(LVLS);
	delete LVLS;
}

void Hud::eBus_setup(){
	// Hook handleEvents into event handler
	this->f_eHandler = new EBus_Fn( std::bind(&Hud::handleEvents, this, std::placeholders::_1) );
	// Specify event subscriptions
	g_eventbus.subscribe("player.obstacles.update", f_eHandler);
	g_eventbus.subscribe("player.health.update", f_eHandler);
	g_eventbus.subscribe("game.state.set", f_eHandler);
}

void Hud::font_create(){

			g_rc.import_texture("font", "./assets/boxy_bold_font_linear.png");

			// Crop mappings. 1: x, 2: w
			std::map<char, std::pair<int, int>> xcrops;
			xcrops['!'] = std::pair<int,int> (0,5);
			xcrops['"'] = std::pair<int,int> (5,5);
			xcrops['#'] = std::pair<int,int> (13,10);
			xcrops['$'] = std::pair<int,int> (23,8);
			xcrops['%'] = std::pair<int,int> (31,11);
			xcrops['&'] = std::pair<int,int> (42,10);
			xcrops['\''] = std::pair<int,int>(52,5);
			xcrops['('] = std::pair<int,int> (57,5);
			xcrops[')'] = std::pair<int,int> (63,6);
			//xcrops['*'] = std::pair<int,int> (69,7);
			//xcrops['+'] = std::pair<int,int> (76,9);
			xcrops[','] = std::pair<int,int> (85,6);
			//xcrops['-'] = std::pair<int,int> (91,7);
			xcrops['.'] = std::pair<int,int> (98,5);
			//xcrops['/'] = std::pair<int,int> (103,7);

			xcrops['0'] = std::pair<int,int> (110,8);
			xcrops['1'] = std::pair<int,int> (118,5);
			xcrops['2'] = std::pair<int,int> (123,8);
			xcrops['3'] = std::pair<int,int> (131,8);
			xcrops['4'] = std::pair<int,int> (139,8);
			xcrops['5'] = std::pair<int,int> (147,8);
			xcrops['6'] = std::pair<int,int> (155,8);
			xcrops['7'] = std::pair<int,int> (163,8);
			xcrops['8'] = std::pair<int,int> (171,8);
			xcrops['9'] = std::pair<int,int> (179,8);

			xcrops[':'] = std::pair<int,int> (187,5);
			xcrops[';'] = std::pair<int,int> (192,5);
			xcrops['<'] = std::pair<int,int> (197,7);
			xcrops['='] = std::pair<int,int> (204,7);
			xcrops['>'] = std::pair<int,int> (211,7);
			xcrops['?'] = std::pair<int,int> (218,9);
			xcrops['@'] = std::pair<int,int> (227,9);

			xcrops['A'] = std::pair<int,int> (236, 8);
			xcrops['B'] = std::pair<int,int> (244, 8);
			xcrops['C'] = std::pair<int,int> (252, 8);
			xcrops['D'] = std::pair<int,int> (260, 8);
			xcrops['E'] = std::pair<int,int> (268, 8);
			xcrops['F'] = std::pair<int,int> (276, 8);
			xcrops['G'] = std::pair<int,int> (284, 8);
			xcrops['H'] = std::pair<int,int> (292, 8);
			xcrops['I'] = std::pair<int,int> (300, 5);
			xcrops['J'] = std::pair<int,int> (305, 8);
			xcrops['K'] = std::pair<int,int> (313, 8);
			xcrops['L'] = std::pair<int,int> (321, 8);
			xcrops['M'] = std::pair<int,int> (329, 10);
			xcrops['N'] = std::pair<int,int> (339, 9);
			xcrops['O'] = std::pair<int,int> (348, 8);
			xcrops['P'] = std::pair<int,int> (356, 8);
			xcrops['Q'] = std::pair<int,int> (364, 9);
			xcrops['R'] = std::pair<int,int> (373, 8);
			xcrops['S'] = std::pair<int,int> (381, 8);
			xcrops['T'] = std::pair<int,int> (389, 9);
			xcrops['U'] = std::pair<int,int> (398, 8);
			xcrops['V'] = std::pair<int,int> (406, 8);
			xcrops['W'] = std::pair<int,int> (414, 10);
			xcrops['X'] = std::pair<int,int> (424, 8);
			xcrops['Y'] = std::pair<int,int> (432, 9);
			xcrops['Z'] = std::pair<int,int> (441, 8);

			xcrops['a'] = std::pair<int,int> (236, 8);
			xcrops['b'] = std::pair<int,int> (244, 8);
			xcrops['c'] = std::pair<int,int> (252, 8);
			xcrops['d'] = std::pair<int,int> (260, 8);
			xcrops['e'] = std::pair<int,int> (268, 8);
			xcrops['f'] = std::pair<int,int> (276, 8);
			xcrops['g'] = std::pair<int,int> (284, 8);
			xcrops['h'] = std::pair<int,int> (292, 8);
			xcrops['i'] = std::pair<int,int> (300, 5);
			xcrops['j'] = std::pair<int,int> (305, 8);
			xcrops['k'] = std::pair<int,int> (313, 8);
			xcrops['l'] = std::pair<int,int> (321, 8);
			xcrops['m'] = std::pair<int,int> (329, 10);
			xcrops['n'] = std::pair<int,int> (339, 9);
			xcrops['o'] = std::pair<int,int> (348, 8);
			xcrops['p'] = std::pair<int,int> (356, 8);
			xcrops['q'] = std::pair<int,int> (364, 9);
			xcrops['r'] = std::pair<int,int> (373, 8);
			xcrops['s'] = std::pair<int,int> (381, 8);
			xcrops['t'] = std::pair<int,int> (389, 9);
			xcrops['u'] = std::pair<int,int> (398, 8);
			xcrops['v'] = std::pair<int,int> (406, 8);
			xcrops['w'] = std::pair<int,int> (414, 10);
			xcrops['x'] = std::pair<int,int> (424, 8);
			xcrops['y'] = std::pair<int,int> (432, 9);
			xcrops['z'] = std::pair<int,int> (441, 8);

			xcrops['['] = std::pair<int,int> (449, 6);
			xcrops['\\'] = std::pair<int,int>(455, 7);
			xcrops[']'] = std::pair<int,int> (462, 6);
			//xcrops['^'] = std::pair<int,int> (468, 9);
			xcrops['_'] = std::pair<int,int> (477, 7);
			xcrops['`'] = std::pair<int,int> (484, 6);
			xcrops['{'] = std::pair<int,int> (490, 6);
			xcrops['|'] = std::pair<int,int> (496, 5);
			xcrops['}'] = std::pair<int,int> (501, 6);
			xcrops['~'] = std::pair<int,int> (507, 10);


			for(auto &i : xcrops){
				std::string letter_str(1, i.first);
				auto lettersprite = g_rc.make_static_sprite_from_texture("font."+letter_str, "font", Z_PlaneMeta {
					.u = (float)i.second.first, .v = 0, .uw = (float)i.second.second, .vw = 9
				}).second;
				gameOverText->add_tile(i.first, lettersprite);
				gameIntroText->add_tile(i.first, lettersprite);
				gameWaveCooldownText->add_tile(i.first, lettersprite);
				tm_inventory->add_tile(i.first, lettersprite);
			};
}

void Hud::text_layers_create(){
	this->gameOverText = new Tilemap(24,32);
	gameOverText->write(std::pair(10, 10), "   GAME OVER\r\n\r\nRETURN   Restart\r\nESCAPE      Quit");
	this->gameOverText->visible = false;
	_board.attach(gameOverText);

	this->gameIntroText = new Tilemap(16, 24);
	gameIntroText->write(std::pair(18, 5), "FLOWERDEFENSE\r\n\r\nRETURN   Start\r\nESCAPE   Quit\r\n\r\nW        Move UP\r\nA        Move LEFT\r\nS        Move DOWN\r\nD        Move RIGHT\r\n\r\nUP       Attack Up\r\nLEFT     Attack Left\r\nDOWN     Attack Down\r\nRIGHT    Attack Right\r\n\r\nSPACE    Place Fence\r\n\r\n\r\n\r\nDebug\r\n\r\nF1       Spritebox\r\nF2       Hitbox\r\nF3       BFS Player\r\nF4       BFS Flower\r\nF9       SHOW ENGINE LOG");
	this->gameIntroText->visible = true;
	_board.attach(gameIntroText);

	this->gameWaveCooldownText = new Tilemap(16, 24);
	gameWaveCooldownText->write(std::pair(18, 5), "Next Wave in\r\n6 Seconds");
	this->gameWaveCooldownText->visible = false;
	_board.attach(gameWaveCooldownText);
}
/*



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

*/

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

	auto a_heart = g_rc.add_anim("player.health.4_4", gaertner_leben_1);
	auto t_heart = g_rc.make_sprite_from_anim("player.health.4_4", "player.health.4_4", Z_PlaneMeta { .x = 0, .y = 32, .w = 32, .h = 32 });
	auto t_obstacle = g_rc.get_sprite("tiles.obstacle");
	auto t_xp = g_rc.make_sprite_from_anim("player.health.4_4", "player.health.4_4", Z_PlaneMeta { .x = 0, .y = 32, .w = 32, .h = 32 });
	tm_inventory =  new Tilemap(24,24);

	tm_inventory->add_tile('+', t_heart.second);
	tm_inventory->add_tile('^', t_obstacle);

	tm_inventory->fill(0, 0, 16, 2, ' ');
	tm_inventory->write(std::pair(0,1), "+X");
	tm_inventory->write(std::pair(0,2), "^X");

	_board.attach(tm_inventory);
}

void Hud::rose_leben_create(){
			 rose_leben_a[0] = new Animation(g_rc.get_texture("spritesheet"), 1);
			 rose_leben_a[1] = new Animation(g_rc.get_texture("spritesheet"), 1);
			 rose_leben_a[2] = new Animation(g_rc.get_texture("spritesheet"), 1);
			 rose_leben_a[3] = new Animation(g_rc.get_texture("spritesheet"), 1);
			 rose_leben_a[4] = new Animation(g_rc.get_texture("spritesheet"), 1);


		

			rose_leben_a[4]->add_frame(Z_PlaneMeta{ .u = 32 * 0, .v = 32 * 4, .uw = 32, .vw = 32 });
			//rose_leben_a[4]->add_frame(Z_PlaneMeta{ .pivot_x = 16, .pivot_y = 16, .deg = 30, .u = 32 * 0, .v = 32 * 4, .uw = 32, .vw = 32 });
			rose_leben_a[3]->add_frame(Z_PlaneMeta{ .u = 32 * 1, .v = 32 * 4, .uw = 32, .vw = 32 });
			//rose_leben_a[3]->add_frame(Z_PlaneMeta{ .pivot_x = 16, .pivot_y = 16, .deg = 30, .u = 32 * 1, .v = 32 * 4, .uw = 32, .vw = 32 });
			rose_leben_a[2]->add_frame(Z_PlaneMeta{ .u = 32 * 2, .v = 32 * 4, .uw = 32, .vw = 32 });
			//rose_leben_a[2]->add_frame(Z_PlaneMeta{ .pivot_x = 16, .pivot_y = 16, .deg = 30, .u = 32 * 2, .v = 32 * 4, .uw = 32, .vw = 32 });
			rose_leben_a[1]->add_frame(Z_PlaneMeta{ .u = 32 * 3, .v = 32 * 4, .uw = 32, .vw = 32 });
			//rose_leben_a[1]->add_frame(Z_PlaneMeta{ .pivot_x = 16, .pivot_y = 16, .deg = 30, .u = 32 * 3, .v = 32 * 4, .uw = 32, .vw = 32 });
			rose_leben_a[0]->add_frame(Z_PlaneMeta{ .u = 32 * 4, .v = 32 * 4, .uw = 32, .vw = 32 });
			//rose_leben_a[0]->add_frame(Z_PlaneMeta{ .pivot_x = 16, .pivot_y = 16, .deg = 30, .u = 32 * 4, .v = 32 * 4, .uw = 32, .vw = 32 });



			rose_leben_a[4]->add_xsheet_phase(0, 1);
			rose_leben_a[3]->add_xsheet_phase(0, 1);
			rose_leben_a[2]->add_xsheet_phase(0, 1);
			rose_leben_a[1]->add_xsheet_phase(0, 1);
			rose_leben_a[0]->add_xsheet_phase(0, 1);


		
			g_rc.add_anim("rose_leben.rose_leben_5", rose_leben_a[4]);
			g_rc.add_anim("rose_leben.rose_leben_4", rose_leben_a[3]);
			g_rc.add_anim("rose_leben.rose_leben_3", rose_leben_a[2]);
			g_rc.add_anim("rose_leben.rose_leben_2", rose_leben_a[1]);
			g_rc.add_anim("rose_leben.rose_leben_1", rose_leben_a[0]);

			 spr_rose_leben = g_rc.make_sprite("rose_leben", Z_PlaneMeta { .x = 32*23, .y = 32, .w = 64, .h = 64}).second;
			 spr_rose_leben->add_animation("rose_leben_1", rose_leben_a[0]);
			 spr_rose_leben->add_animation("rose_leben_2", rose_leben_a[1]);
			 spr_rose_leben->add_animation("rose_leben_3", rose_leben_a[2]);
			 spr_rose_leben->add_animation("rose_leben_4", rose_leben_a[3]);
			 spr_rose_leben->add_animation("rose_leben_5", rose_leben_a[4]);
			 //rose_leben[4] = g_rc.make_sprite_from_anim("rose_leben", "rose_leben.rose_leben_5", Z_PlaneMeta { .x = 32*23, .y = 32, .w = 64, .h = 64}).second;
			 //rose_leben[3] = g_rc.make_sprite_from_anim("rose_leben", "rose_leben.rose_leben_4", Z_PlaneMeta { }).second;
			 //rose_leben[2] = g_rc.make_sprite_from_anim("rose_leben", "rose_leben.rose_leben_3", Z_PlaneMeta { }).second;
			 //rose_leben[1] = g_rc.make_sprite_from_anim("rose_leben", "rose_leben.rose_leben_2", Z_PlaneMeta {  }).second;
			 //rose_leben[0] = g_rc.make_sprite_from_anim("rose_leben", "rose_leben.rose_leben_1", Z_PlaneMeta { }).second;
			
			 this->attach_rose_leben();
			
			//_board.attach(rose_leben[0]);
			//_board.attach(rose_leben[1]);
			//_board.attach(rose_leben[2]);
			//_board.attach(rose_leben[3]);
			//_board.attach(rose_leben[4]);
}

void Hud::attach_rose_leben()
{
	_board.attach(spr_rose_leben);
}

void Hud::dettach_rose_leben()
{
	_board.detach(spr_rose_leben);
}

void Hud::rose_update_health(){
	float frac_rose_leben = (float) rose_akt_leben / (float) rose_max_leben;
	if(frac_rose_leben >= 0.75f){
		spr_rose_leben->switch_to_anim("rose_leben_5");
		return;
	}
	if(frac_rose_leben >= 0.5f){
		spr_rose_leben->switch_to_anim("rose_leben_4");
		return;
	}
	if(frac_rose_leben >= 0.25f){
		spr_rose_leben->switch_to_anim("rose_leben_3");
		return;
	}
	if(frac_rose_leben >= 0.0f){
		spr_rose_leben->switch_to_anim("rose_leben_2");
		return;
	}
	spr_rose_leben->switch_to_anim("rose_leben_1");
	return;
}


void Hud::rose_leben_runter(){
	switch (rose_akt_leben)
	{
	case 5:
		rose_leben[3]->setTransform(Z_PlaneMeta{});
		rose_leben[4]->setTransform(Z_PlaneMeta{ .x = 32 * 23, .y = 32, .w = 64, .h = 64 });
		rose_akt_leben--;
		return;
	case 4:
		rose_leben[4]->setTransform(Z_PlaneMeta{});	
		rose_leben[3]->setTransform(Z_PlaneMeta{.x = 32*23, .y = 32, .w = 64, .h = 64});
		rose_akt_leben--;
		return;
	case 3:
		rose_leben[3]->setTransform(Z_PlaneMeta{});	
		rose_leben[2]->setTransform(Z_PlaneMeta{.x = 32*23, .y = 32, .w = 64, .h = 64});
		rose_akt_leben--;
		return;
	case 2:
		rose_leben[2]->setTransform(Z_PlaneMeta{});	
		rose_leben[1]->setTransform(Z_PlaneMeta{.x = 32*23, .y = 32, .w = 64, .h = 64});
		rose_akt_leben--;
		return;
	case 1:
		rose_leben[1]->setTransform(Z_PlaneMeta{});	
		rose_leben[0]->setTransform(Z_PlaneMeta{.x = 32*23, .y = 32, .w = 64, .h = 64});
		rose_akt_leben--;
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
	switch (gaertner_akt_leben)
	{
	case 4:
		gaertner_leben[3]->setTransform(Z_PlaneMeta{});	
		gaertner_akt_leben--;
		return;
	case 3:
		gaertner_leben[2]->setTransform(Z_PlaneMeta{});	
		gaertner_akt_leben--;
		return;
	case 2:
		gaertner_leben[1]->setTransform(Z_PlaneMeta{});	
		gaertner_akt_leben--;
		return;
	case 1:
		//game over
		break;
	
	default:
		break;
	}
}

void Hud::gaertner_leben_hoch(){
	switch (gaertner_akt_leben)
	{
	case 1:
		gaertner_leben[1]->setTransform(Z_PlaneMeta{.x = 32 * 1, .y = 32, .w = 32, .h = 32 });

		gaertner_akt_leben++;
		return;
	case 2:
		gaertner_leben[2]->setTransform(Z_PlaneMeta{.x = 32 * 2, .y = 32, .w = 32, .h = 32 });
		gaertner_akt_leben++;
		return;
	case 3:
		gaertner_leben[3]->setTransform(Z_PlaneMeta{.x = 32 * 3, .y = 32, .w = 32, .h = 32 });
		gaertner_akt_leben++;
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

void Hud::handleEvents(Event* e){
	if(e->get("type") == "player.obstacles.update"){
		tm_inventory->fill(0, 2, 15, 1, ' ');
		tm_inventory->write(std::pair(0,2), "^"+e->get("count"));
	}
	if(e->get("type") == "player.health.update"){
		// Three fields: "current", "max" & 
		tm_inventory->fill(0, 2, 15, 1, ' ');
		tm_inventory->write(std::pair(0,2), "+"+e->get("count"));
		// TODO: Zelda-Style Quarter-Heart generator
	};
	if(e->get("type") == "game.state.set"){
		DEBUG_MSG("HUD: Scene change caught")
		if (e->get("scene") == "gameintro") {
			gameIntroText->visible = true;
			gameOverText->visible = false;
		}
		if(e->get("scene") == "gameover"){
			gameOverText->visible = true;
			gameIntroText->visible = false;
		}
		if(e->get("scene") == "game"){
			// Reset everything
			gameIntroText->visible = false;
			gameOverText->visible = false;
			wave = 1;
		}
	}
};

void Hud::Update(Player &player,  Enemypool &enemypool){
	// Update gaertner health display
	//tm_inventory->fill(0, 1, 15, 3, ' ');

	// Right pad life counterer
	std::string str_rose_leben = std::to_string(rose_akt_leben);
	if(str_rose_leben.length() < 5)
		str_rose_leben.insert(0, 5 - str_rose_leben.length(), ' ');

	tm_inventory->clear_map();
	if( player.health > 5){
		tm_inventory->write(std::pair(0,2), "+"+std::to_string(player.health));
	}else{
		tm_inventory->fill(0, 2, player.health, 1, '+');
	}
	tm_inventory->write(std::pair(28,4), str_rose_leben);
	tm_inventory->write(std::pair(0,3), "^"+std::to_string(player.obstacles));
	tm_inventory->write(std::pair(0, 30), "Mobs: " + std::to_string(enemiesInWaveLeft));
	tm_inventory->write(std::pair(0, 31), "XpLv: "+std::to_string(player.akt_LV));
	tm_inventory->write(std::pair(0, 32), "Wave: " + std::to_string(wave));

	ex_bar_steuern(player.xp_bar);
	if(player.xp_bar>= 1000){
		std::cout<<"############## XP voll"<<std::endl;
		player.xp_bar = 200;
		player.xp_bekommt = player.xp_bekommt/ 1.75;

		if(player.xp_bekommt <= 0){
			player.xp_bekommt = 10;
		}
		player.akt_LV++;
		player.lvl_up = true;
		ausgewahelt = true;

		g_game.state = EnumGameState::SKILLSELECT;
	}

	if(ausgewahelt == true){
		option(player, enemypool);		
	}

	if (g_game.state != EnumGameState::PLAY) {
		ex_bar->setTransform(Z_PlaneMeta{ .x = -INFINITY, .y = -INFINITY });
	}

	player.press = false;

}

bool Hud::option(Player &player,  Enemypool &enemypool){

			LVLS->visible = true;
			g_game.play(g_rc.get_sound("lvlup"));



if(player.press == true){
	std::cout<< "----"<<std::endl;
	std::cout<< "----"<<std::endl;

	auto lvls_trans = LVLS->get_transform();
	auto opt_coord = LVLS->get_coordinate_from_offset(player.mouseX - lvls_trans.x, player.mouseY - lvls_trans.y);

	if(opt_coord.first == 1 && player.press == true){
		std::cout<< "--option 1--"<<std::endl;
		player.option_wahl = 1;
		player.press = false;
		ausgewahelt = false;
		return false;
	}else if(opt_coord.first == 2 && player.press == true){
		std::cout<< "--option 2--"<<std::endl;
		player.option_wahl = 2;
		player.press = false;
		ausgewahelt = false;
		return false;
	}else if(opt_coord.first == 3 && player.press == true){
		std::cout<< "--option 3--"<<std::endl;
		player.option_wahl = 3;
		player.press = false;
		ausgewahelt = false;
		return false;
	}
	}
return true;
}
