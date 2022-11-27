#pragma once
#include "headers.hh"

class BFS {
	ResourceManager& _rc;
	Plane& _board;
	EventBus& _eb;
	KeyMapper& _keymap;
	EBus_Fn f_set_visibility;
	Event* e_bfs_visibility;
	bool isAttached = false;
	bool isValid(const std::vector<std::vector<bool>>& visited, const int& x, const int& y, const int& cellCountX, const int& cellCountY);
public:
	Tilemap* bfsArrows;
	BFS(ResourceManager& rc, EventBus& eb, KeyMapper& keymap, Plane& board);
	void execute(const std::pair<int, int>& root);
	void execute(const int& x, const int& y);
	~BFS();
};