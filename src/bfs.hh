#pragma once
#include "headers.hh"

class BFS {
	Plane& _board;
	Tilemap& _obstacles;
	EBus_Fn f_set_visibility;
	Event* e_bfs_visibility;
	bool isAttached = false;
	bool isValid(const std::vector<std::vector<bool>>& visited, const int& x, const int& y, const int& cellCountX, const int& cellCountY, const std::vector<std::vector<char>>& charVec);
public:
	Tilemap* bfsArrows;
	BFS(Plane& board, Tilemap& obstacles);
	void execute(const std::pair<int, int>& root);
	void execute(const int& x, const int& y);
	~BFS();
};