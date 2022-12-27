#pragma once
#include "headers.hh"

class BFS {
	Tilemap& _obstacles;
	bool isValid(const std::vector<std::vector<bool>>& visited, const int& x, const int& y, const int& cellCountX, const int& cellCountY, const std::vector<std::vector<char>>& charVec);
public:
	bool isAttached = false;
	Tilemap* bfsArrows;
	SDL_Point goalTileCoordinates = { .x = 0, .y = 0 };
	BFS(Tilemap& obstacles, std::string id);
	void execute(const std::pair<int, int>& root);
	void execute(const int& x, const int& y);
	~BFS();
	static constexpr char scanDir[8]			= { 't', 'b', 'l', 'r', '1', '4', '3', '2' };
	static constexpr char traverseDir[8]		= { 'b', 't', 'r', 'l', '4', '1', '2', '3' };
};
