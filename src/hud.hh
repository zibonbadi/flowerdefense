#pragma once
#include "headers.hh"
#include "player.hh"

class Player;
class Hud{
public:
	Plane &_board ;
	Animation* ex_balken_anzeige;
	Animation* gaertner_leben_1;
	Animation* rose_leben_a[4];
	Animation* ex_anzeige;
	EBus_Fn* f_eHandler;
	Sprite *ex_bar;
	Sprite *ex;
	Sprite *gaertner_leben[4];
	Sprite *rose_leben[4];
	Tilemap *ex_rahmen, *tm_inventory , *text;

	int gaetner_akt_leben = 4;
	int rose_akt_leben =4;

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
	void rose_leben_runter();
	void rose_leben_hoch();
	void ex_bar_steuern(float w);
	void exp_create(float x, float y);
	void text_layer_create();
	void font_create();
	void Update(Player &player);

};
