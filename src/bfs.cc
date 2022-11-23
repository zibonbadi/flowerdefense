#include "bfs.hh"
#include <sstream>

void BFS::execute(const std::pair<int, int>& root) {
	BFS::execute(root.first, root.second);
}

void BFS::execute(const int& x_root, const int& y_root) {
	//char cAtPos95 = arrowMap.get_spot(19, 5);

	//std::map<std::pair<unsigned int, unsigned int>, char> map = bfsArrows->get_map();

	//arrowMap.write(std::pair{ 1,1 }, "t");

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
	const char reverseDir[] =	{'b','t','r','l','4','1','2','3' };

	//std::map<std::pair<unsigned int, unsigned int>, bool> visited;
	std::vector<std::vector<bool>> visited;
	std::vector<std::vector<char>> charVec;

	visited.resize(cellCountY);
	charVec.resize(cellCountY);
	for (int i = 0; i < cellCountY; i++) {
		visited[i].resize(cellCountX, false);
		charVec[i].resize(cellCountX, 0);
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

		// Go to the adjacent cells

		for (int i = 0; i < 8; i++) {

			const int& adjx = x + deltaX[i];
			const int& adjy = y + deltaY[i];
			//std::cout << "i: " << i << "    " << x << " , " << y << std::endl;

			if (isValid(visited, adjx, adjy, cellCountX, cellCountY)) {
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
bool BFS::isValid(const  std::vector<std::vector<bool>>& visited, const int& x, const int& y, const int& cellCountX, const int& cellCountY)
{
	// If cell lies out of bounds
	if (x < 0 || y < 0
		|| x >= cellCountX || y >= cellCountY)
		return false;


	// If cell is already visited
	if (visited[y][x])
		return false;

	// Otherwise
	return true;
}

BFS::BFS(ResourceManager& rc, EventBus& eb, KeyMapper& keymap, Plane& board) : _rc(rc), _eb(eb), _keymap(keymap), _board(board) {
	/* Breadth First Search Tilemap */
	bfsArrows = new Tilemap(16, 16);
	auto dd = bfsArrows->get_transform();
	/* Select tiles from tileset */
	auto arrowTop = rc.make_static_sprite_from_texture("tiles.arrowTop", "spritesheet", Z_PlaneMeta{ .u = 32 * 0, .v = 32 * 7, .uw = 32, .vw = 32 }).second;
	auto arrowTopLeft = rc.make_static_sprite_from_texture("tiles.arrowTopLeft", "spritesheet", Z_PlaneMeta{ .u = 32 * 1, .v = 32 * 7, .uw = 32, .vw = 32 }).second;
	auto arrowLeft = rc.make_static_sprite_from_texture("tiles.arrowLeft", "spritesheet", Z_PlaneMeta{ .u = 32 * 2, .v = 32 * 7, .uw = 32, .vw = 32 }).second;
	auto arrowBottomLeft = rc.make_static_sprite_from_texture("tiles.arrowBottomLeft", "spritesheet", Z_PlaneMeta{ .u = 32 * 3, .v = 32 * 7, .uw = 32, .vw = 32 }).second;
	auto arrowBottom = rc.make_static_sprite_from_texture("tiles.arrowBottom", "spritesheet", Z_PlaneMeta{ .u = 32 * 4, .v = 32 * 7, .uw = 32, .vw = 32 }).second;
	auto arrowBottomRight = rc.make_static_sprite_from_texture("tiles.arrowBottomRight", "spritesheet", Z_PlaneMeta{ .u = 32 * 5, .v = 32 * 7, .uw = 32, .vw = 32 }).second;
	auto arrowRight = rc.make_static_sprite_from_texture("tiles.arrowRight", "spritesheet", Z_PlaneMeta{ .u = 32 * 6, .v = 32 * 7, .uw = 32, .vw = 32 }).second;
	auto arrowTopRight = rc.make_static_sprite_from_texture("tiles.arrowTopRight", "spritesheet", Z_PlaneMeta{ .u = 32 * 7, .v = 32 * 7, .uw = 32, .vw = 32 }).second;


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
	_keymap.bind(SDLK_k, *e_bfs_visibility);

	// Register event
	_eb.subscribe("bfs.set_visibility", &f_set_visibility);
}

BFS::~BFS() {
	delete bfsArrows;
}
