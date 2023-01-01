#pragma once
#include "headers.hh"

class Hud{
public:
	Plane &_board ;
	Animation* ex_balken_anzeige;
	Animation* gaertner_leben_1;
	Animation* rose_leben_a[5];
	Animation* ex_anzeige;
	Animation* LVL_anzeige;
	EBus_Fn* f_eHandler;
	Sprite *ex_bar;
	Sprite *ex;
	Sprite *gaertner_leben[4];
	Sprite *rose_leben[5];
	Sprite *spr_rose_leben;
	Sprite *LVLSY[3];
	Tilemap *ex_rahmen, *tm_inventory , *gameOverText, *gameIntroText, *gameWaveCooldownText, *LVLS;

	int gaertner_akt_leben = 4;
	int rose_max_leben =50;
	int rose_akt_leben =50;
	int xp_akt = 0;
	int wave = 1;
	int enemiesInWaveLeft = -1;
	bool ausgewahelt = false;

	Hud(Plane &board);
	~Hud();
	// Hud();
	// void setResourceManager(ResourceManager &rc) { _rc = rc;}
	// void setPlane(Plane &board){_board = board;}
	void eBus_setup();
	void handleEvents(Event* e);
	void ex_rahmen_create();
	void ex_bar_create();
	void gaertner_leben_create();
	void rose_leben_create();
	void attach_rose_leben();
	void dettach_rose_leben();
	void gaertner_leben_runter();
	void gaertner_leben_hoch();
	void rose_update_health();
	void rose_leben_runter();
	void rose_leben_hoch();
	void ex_bar_steuern(float w);
	void exp_create(float x, float y);
	void text_layers_create();
	void font_create();
	void Update(Player &player,  Enemypool &enemypool);

	bool option(Player &player,  Enemypool &enemypool);
};
