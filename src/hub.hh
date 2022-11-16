#ifndef HUB_H
#define HUB_H

#include "headers.hh"

class Hub{
private:
	ResourceManager &_rc;
	Plane &_board;
	Animation* ex_balken_anzeige;
	Animation* gaertner_leben_1;
	Animation* rose_leben_a[4];
public:
	Hub(ResourceManager &rc, Plane &board) : _rc(rc), _board(board){}
	void ex_rahmen_create();
	void ex_bar_create();
	void gaertner_leben_create();
	void rose_leben_create();
	void gaertner_leben_runter();
	void gaertner_leben_hoch();
	void rose_leben_runter();
	void rose_leben_hoch();
	void ex_bar_steuern(float w);

	

	Sprite *ex_bar;
	Sprite *gaertner_leben[4];
	Sprite *rose_leben[4];
	Tilemap *ex_rahmen;

	int gaetner_akt_leben = 4;
	int rose_akt_leben =4;

};
#endif
