#include <SDL2/SDL.h>
#include "board.h"
#include "utilities.h"

Board::Board() {
	// set every element in board to EMPTY
	for (auto& index : board) {
		index = EMPTY;
	}
	// set up the tile
	tile.x = tile.y = 0;
	tile.w = TILE_SIZE;
	tile.h = TILE_SIZE;
}

void Board::drawWalls() {
	// set the colour to green
	SDL_SetRenderDrawColor(ren, 0, 255, 0, 255);
	int gridRef{ 0 };
	// for every element in board
	for (auto index : board) {
		if (index == WALL) { // draw it if its a wall
			this->drawNode(gridRef);
		}
		gridRef++;
	}
}

void Board::drawPaths() {
	int gridRef{ 0 };
	// for every element in board
	for (auto index : board) {
		// don't draw the special Nodes, only pathing Nodes
		if (index != END && index != WALL && index != EMPTY) {
			// set the colour to a scaled value depending on the Node
			SDL_SetRenderDrawColor(ren, 0, 0, 30 * index, 100);
			// draw the Node
			this->drawNode(gridRef);
		}
		gridRef++;
	}
}

void Board::animatePaths() {
	static Uint8 ripple{ 8 };
	static Uint8 rippleOffset{ 0 };
	(rippleOffset >= ripple) ? rippleOffset = 0 : rippleOffset++;
	int gridRef{ 0 };
	// for every element in board
	for (auto index : board) {
		// don't draw the special Nodes, only pathing Nodes that match the ripple offset this time round
		if (index != END && index != WALL && index != EMPTY && ((index - rippleOffset) % ripple) == 0) {
			// set the colour to a scaled value depending on the Node
			SDL_SetRenderDrawColor(ren, 50, 50, 255, 200);
			// draw the Node
			this->drawNode(gridRef);
		}
		gridRef++;
	}
}

void Board::clearPathingValues() {
	int gridRef{ 0 };
	// for every element in board
	for (auto index : board) {
		// draw it if its a wall
		if (index != WALL) {
			this->set(gridRef, EMPTY);
		}
		gridRef++;
	}
}

void Board::clearWalls() {
	int gridRef{ 0 };
	// for every element in board
	for (auto index : board) {
		// draw it if its a wall
		if (index == WALL) {
			this->set(gridRef, EMPTY);
		}
		gridRef++;
	}
}

void Board::drawNode(int x, int y) {
	tile.x = x * TILE_SIZE;
	tile.y = y * TILE_SIZE;
	// draw it to the screen
	SDL_RenderFillRect(ren, &tile);
}

void Board::drawNode(int gridRef) {
	// get the coords from the grid reference
	tile.x = gridRef % GRID_WIDTH;
	gridRef -= tile.x;
	tile.y = gridRef / GRID_WIDTH;
	// scale the coords so they are positioned correctly on screen
	tile.x *= TILE_SIZE;
	tile.y *= TILE_SIZE;
	// draw it to the screen
	SDL_RenderFillRect(ren, &tile);
}

void Board::drawNode(const Node& node) {
	this->drawNode(node.x, node.y);
}

Uint16 Board::get(int x, int y) const{
	clamp(0, GRID_WIDTH  - 1, x);
	clamp(0, GRID_HEIGHT - 1, y);
	return board[(GRID_WIDTH * y) + x];
}

Uint16 Board::get(int gridRef) const{
	return board[gridRef];
}

Uint16 Board::get(Node node) const {
	return this->get(node.x, node.y);
}

Uint16 Board::getRelative(int x, int y) const{
	return this->get( (x / TILE_SIZE), (y / TILE_SIZE) );
}


void Board::assignIndexTo(Node& node) {
	node.i = this->get(node.x, node.y);
}

// sets the board location x y to value
void Board::set(int x, int y, Uint16 index) {
	clamp(0, GRID_WIDTH, x);
	clamp(0, GRID_HEIGHT, y);
	board[(GRID_WIDTH) * y + x] = index;
}

void Board::set(int gridRef, Uint16 index) {
	clamp(0, GRID_HEIGHT * GRID_WIDTH, gridRef);
	board[gridRef] = index;
}

void Board::set(const Node& node) {
	this->set(node.x, node.y, node.i);
}

void Board::setRelative(int x, int y, Uint16 value) {
	this->set(x / TILE_SIZE, y / TILE_SIZE, value);
}
