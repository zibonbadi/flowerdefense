#include "bfs.hh"
#include <sstream>
#include <algorithm>
#include <random>

void BFS::execute(const std::pair<int, int>& root) {
	BFS::execute(root.first, root.second);
}

void BFS::execute(const int& x_root, const int& y_root) {
	//char cAtPos95 = arrowMap.get_spot(19, 5);

	//std::map<std::pair<unsigned int, unsigned int>, char> map = bfsArrows->get_map

	Z_PlaneMeta zpMeta = bfsArrows->get_transform();
	int cellCountX = SCREEN_WIDTH / zpMeta.w;		// 800 / 16 = 50
	int cellCountY = SCREEN_HEIGHT / zpMeta.h;		// 800 / 16 = 50

	// Possible arrow pointing direction
	// towards one of its eight neighburs:
	// 
	// legend
	// t: top
	// b: bottom
	// l: left
	// r: right
	// 
	// tl  t  tr
	// l      r
	// bl  b  br
	// 
	// tl, tr, bl and br will be encoded tl = '1', tr = '2',  bl = '3' and br = '4'
	// because this games engine only supports chars as tile keys:
	//
	// 1  t  2
	// l     r
	// 3  b  4

	// Direction vectors
								// t,  b,  l,  r, tl, br, bl, tr
	const int deltaX[] =		{  0,  0, -1,  1, -1,  1, -1,  1 };
	const int deltaY[] =		{ -1,  1,  0,  0, -1,  1,  1, -1 };
	const char reverseDir[] =	{'b','t','r','l','4','1','2', '3'};
	std::vector<int> order =	{  0,  1,  2,  3,  4,  5,  6,  7 };
	std::random_device rd;
	std::mt19937 g(rd());

	

	//std::map<std::pair<unsigned int, unsigned int>, bool> visited;
	std::vector<std::vector<bool>> visited;
	std::vector<std::vector<char>> charVec;

	visited.resize(cellCountY);
	charVec.resize(cellCountY);
	for (int i = 0; i < cellCountY; i++) {
		visited[i].resize(cellCountX, false);
		charVec[i].resize(cellCountX, 0);
	}

	std::map<std::pair<unsigned int, unsigned int>, char> obstacles = _obstacles.get_map();
	const float innerTileCount = _obstacles.get_transform().w / zpMeta.w;

	for (auto const& [key, val] : obstacles) {
		const int& outer_x = key.second;
		const int& outer_y = key.first;
		const int& inner_x = outer_x * innerTileCount;
		const int& inner_y = outer_y * innerTileCount;

		if (val == 'x') {
			for (int y = inner_y; y < (inner_y + innerTileCount); y++) {
				for (int x = inner_x; x < (inner_x + innerTileCount); x++) {
					charVec[y][x] = 'x';
				}
			}
		}
	}

	// Stores indices of the matrix cells
	std::queue <std::pair<int, int> > q;

	// Mark the starting cell as visited
	// and push it into the queue
	q.push(std::pair{ x_root, y_root });
	visited[y_root][x_root] = true;

	// Iterate while the queue
	// is not empty
	while (!q.empty()) {

		const std::pair<int, int>& cell = q.front();
		const int x = cell.first;
		const int y = cell.second;
		//std::cout << arrowMap.get_spot(x, y) << " ";

		q.pop();

		std::shuffle(order.begin(), order.end(), g);
		
		// Go to the adjacent cells
		for (int k= 0; k < 8; k++) {
			int i = order[k];

			const int& adjx = x + deltaX[i];
			const int& adjy = y + deltaY[i];
			//std::cout << "i: " << i << "    " << x << " , " << y << std::endl;

			if (isValid(visited, adjx, adjy, cellCountX, cellCountY, charVec)) {
				q.push(std::pair{ adjx, adjy });
				visited[adjy][adjx] = true;

				charVec[adjy][adjx] = reverseDir[i];

				//arrowMap.write(std::pair{ adjx, adjy }, charVec[adjy][adjx] + "");
			}
		}
	}


	/*
			1  procedure BFS(G, root) is
			2      let Q be a queue
			3      label root as explored
			4      Q.enqueue(root)
			5      while Q is not empty do
			6          v : = Q.dequeue()
			7          if v is the goal then
			8              return v
			9          for all edges from v to w in G.adjacentEdges(v) do
			10              if w is not labeled as explored then
			11                  label w as explored
			12                  w.parent : = v
			13                  Q.enqueue(w)
	*/

	std::stringstream ss;

	for (int i = 0; i < charVec.size(); i++) {
		for (int j = 0; j < charVec[i].size(); j++) {
			ss << charVec[i][j];
		}
		ss << "\r\n";
	}
	std::cout << ss.str() << std::endl;
	bfsArrows->write(std::pair{ 0, 0 }, ss.str());
	ss.str("");
}

// Function to check if a cell
// is be visited or not
bool BFS::isValid(const  std::vector<std::vector<bool>>& visited, const int& x, const int& y, const int& cellCountX, const int& cellCountY, const std::vector<std::vector<char>>& charVec)
{
	// If cell lies out of bounds
	if (x < 0 || y < 0
		|| x >= cellCountX || y >= cellCountY)
		return false;


	// If cell is already visited
	if (visited[y][x])
		return false;

	// if cell is an obstacle
	if (charVec[y][x] == 'x')
		return false;

	// Otherwise
	return true;
}

BFS::BFS(Plane& board, Tilemap& obstacles) : _board(board), _obstacles(obstacles) {
	/* Breadth First Seag_resourcemanagerh Tilemap */
	bfsArrows = new Tilemap(BFS_TILE_WIDTH, BFS_TILE_HEIGHT);
	auto dd = bfsArrows->get_transform();
	/* Select tiles from tileset */
	auto arrowTop = g_rc.make_static_sprite_from_texture("tiles.bfs.arrowTop", "spritesheet", Z_PlaneMeta{ .u = 32 * 0, .v = 32 * 7, .uw = 32, .vw = 32 }).second;
	auto arrowTopLeft = g_rc.make_static_sprite_from_texture("tiles.bfs.arrowTopLeft", "spritesheet", Z_PlaneMeta{ .u = 32 * 1, .v = 32 * 7, .uw = 32, .vw = 32 }).second;
	auto arrowLeft = g_rc.make_static_sprite_from_texture("tiles.bfs.arrowLeft", "spritesheet", Z_PlaneMeta{ .u = 32 * 2, .v = 32 * 7, .uw = 32, .vw = 32 }).second;
	auto arrowBottomLeft = g_rc.make_static_sprite_from_texture("tiles.bfs.arrowBottomLeft", "spritesheet", Z_PlaneMeta{ .u = 32 * 3, .v = 32 * 7, .uw = 32, .vw = 32 }).second;
	auto arrowBottom = g_rc.make_static_sprite_from_texture("tiles.bfs.arrowBottom", "spritesheet", Z_PlaneMeta{ .u = 32 * 4, .v = 32 * 7, .uw = 32, .vw = 32 }).second;
	auto arrowBottomRight = g_rc.make_static_sprite_from_texture("tiles.bfs.arrowBottomRight", "spritesheet", Z_PlaneMeta{ .u = 32 * 5, .v = 32 * 7, .uw = 32, .vw = 32 }).second;
	auto arrowRight = g_rc.make_static_sprite_from_texture("tiles.bfs.arrowRight", "spritesheet", Z_PlaneMeta{ .u = 32 * 6, .v = 32 * 7, .uw = 32, .vw = 32 }).second;
	auto arrowTopRight = g_rc.make_static_sprite_from_texture("tiles.bfs.arrowTopRight", "spritesheet", Z_PlaneMeta{ .u = 32 * 7, .v = 32 * 7, .uw = 32, .vw = 32 }).second;

	// Possible arrow pointing direction
	// towards one of its eight neighburs:
	// 
	// legend
	// t: top
	// b: bottom
	// l: left
	// r: right
	// 
	// tl  t  tr
	// l      r
	// bl  b  br
	// 
	// tl, tr, bl and br will be encoded tl = '1', tr = '2',  bl = '3' and br = '4'
	// because this games engine only supports chars as tile keys:
	//
	// 1  t  2
	// l     r
	// 3  b  4

	bfsArrows->add_tile('t', arrowTop);
	bfsArrows->add_tile('l', arrowLeft);
	bfsArrows->add_tile('r', arrowRight);
	bfsArrows->add_tile('b', arrowBottom);
	bfsArrows->add_tile('1', arrowTopLeft);
	bfsArrows->add_tile('2', arrowTopRight);
	bfsArrows->add_tile('3', arrowBottomLeft);
	bfsArrows->add_tile('4', arrowBottomRight);


	f_set_visibility = [&](Event* e) {
		isAttached = !isAttached;
		if (isAttached == true) {
			_board.attach(bfsArrows);
		}
		else {
			_board.detach(bfsArrows);
		}
	};

	e_bfs_visibility = new Event("bfs.set_visibility");
	g_keymapper.bind(SDLK_k, *e_bfs_visibility);

	// Register event
	g_eventbus.subscribe("bfs.set_visibility", &f_set_visibility);
}

BFS::~BFS() {
	delete bfsArrows;
}
