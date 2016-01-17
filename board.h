#ifndef BOARD_H
#define BOARD_H

#include <SDL2/SDL.h>
#include "setup.h"

const int TILE_SIZE{ 16 };
const int GRID_WIDTH{ SCREEN_WIDTH / TILE_SIZE };
const int GRID_HEIGHT{ SCREEN_HEIGHT / TILE_SIZE };
const Uint16 START{ 0 };
const Uint16 END{ 60000 };
const Uint16 EMPTY{ 65500 };
const Uint16 WALL{ 65501 };

extern SDL_Renderer* ren;
extern SDL_Window* win;

// represents a location and index on the board
struct Node{
	Node() {}
	Node(int x, int y) {
		this->x = x;
		this->y = y;
	}
	Uint16 x, y, i;
};

class Board{
public:
	Board();

	// draw the wall only, ignore indexes used for pathfinding
	void drawWalls();
	// draws all the pathing values that are on the board
	void drawPaths();
	// draws the pathing values with a ripple effect
	// this doesn't actually change how the paths are calculated, it's purely visual
	void animatePaths();

	// removes everything that is not a wall
	void clearPathingValues();
	// removes everything that is a wall
	void clearWalls();

	/* getters and setters */
	// returns the value at the specified locaton
	Uint16 get(int x, int y) const;
	Uint16 get(int gridRef) const;
	Uint16 get(Node p) const;
	Uint16 getRelative(int x, int y) const;
	// sets the value of a Node
	void set(int x, int y, Uint16 value);
	// gridref is a one dimensional description of board locations
	// sets the board location at gridref to value
	void set(int gridRef, Uint16 value);
	// sets the board location of Node to the index of Node
	void set(const Node& node);
	// sets the node which the point is over relative to the screen coordinates
	void setRelative(int x, int y, Uint16 value);

	// draws Nodes WITHOUT specifying a draw colour
	void drawNode(int x, int y);
	void drawNode(int gridRef);
	void drawNode(const Node& node);
	// sets the index component of the passed node to the index of the nodes x, y location
	void assignIndexTo(Node& node);
private:
	// a tile to help uf when we're drawing
	SDL_Rect tile;
	// an array to store all the positions on the board

	// SHOULD BE DYNAMICALLY ALLOCATED, UNIQUE POINTER?
	//std::unique_ptr<Uint16> board; // ???

	Uint16 board[GRID_WIDTH*GRID_HEIGHT];
};

#endif
