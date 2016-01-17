#ifndef GOAT_H
#define GOAT_H

#include <SDL2/SDL.h>
#include <iostream>
#include "utilities.h"
#include "board.h"

class Goat {
public:
	Goat() { std::cout << "There is a goat fearing for it's life." << std::endl; }
	~Goat() { std::cout << "The goat died." << std::endl; }

	// draw a square to represent the goat
	void drawPosition();
	void setPosition(int x, int y);

	Node position;

private:

};

#endif
