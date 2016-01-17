#include "goat.h"

void Goat::setPosition(int x, int y) {
	clamp(0, GRID_WIDTH, x);
	clamp(0, GRID_HEIGHT, y);
	position.x = x;
	position.y = y;
}

void Goat::drawPosition() {
	SDL_Rect pos{ position.x * TILE_SIZE, position.y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
	SDL_SetRenderDrawColor(ren, 200, 200, 200, 255);
	SDL_RenderFillRect(ren, &pos);
}