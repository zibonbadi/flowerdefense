#pragma once
#include "headers.hh"

class Hud{
public:
	Plane &_board ;
	Animation* ex_balken_anzeige;
	Animation* gaertner_leben_1;
	Animation* rose_leben_a[5];
	Animation* ex_anzeige;
	EBus_Fn* f_eHandler;
	Sprite *ex_bar;
	Sprite *ex;
	Sprite *gaertner_leben[4];
	Sprite *rose_leben[5];
	Sprite *spr_rose_leben;
	Tilemap *ex_rahmen, *tm_inventory , *text;

	int gaertner_akt_leben = 4;
	int rose_max_leben =50;
	int rose_akt_leben =50;
	int xp_akt = 0;
	int wave = 0;

	Hud(Plane &board);
	// Hud();
	// void setResourceManager(ResourceManager &rc) { _rc = rc;}
	// void setPlane(Plane &board){_board = board;}
	void eBus_setup();
	void handleEvents(Event* e);
	void ex_rahmen_create();
	void ex_bar_create();
	void gaertner_leben_create();
	void rose_leben_create();
	void gaertner_leben_runter();
	void gaertner_leben_hoch();
	void rose_update_health();
	void rose_leben_runter();
	void rose_leben_hoch();
	void ex_bar_steuern(float w);
	void exp_create(float x, float y);
	void text_layer_create();
	void font_create();
	void Update(Player &player);

};
