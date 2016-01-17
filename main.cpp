//#include <SDL.h>
#include <iostream>

#include "utilities.h"
#include "setup.h"
#include "tiger.h"
#include "goat.h"
#include "board.h"
#include "input.h"

/* global variables */
SDL_Window* win;
SDL_Renderer* ren;
SDL_Event event;
Board world;
Tiger tiger(&world, GRID_WIDTH / 2, GRID_HEIGHT / 2);
Input input;
Goat goat;
std::vector<Node> targets;
bool quit{ false };

// time in milliseconds
Uint32 frameStart{ 0 };
Uint32 frameEnd{ 0 };
Uint32 frameTime{ 0 };

int main(int argc, char* argv[]) {
	/* setting up the window */
	std::cout << "launching SDL2 window" << std::endl;
	if ( initSDL()                                != 0 ) { return -1; }
	if ( initWindow(win, "Jungle Simulator 2015") != 0 ) { return -1; }
	if ( initRenderer(win, ren)                   != 0 ) { return -1; }
	std::cout << "window succesfully created" << std::endl;

	goat.position.x = tiger.getPosition().x;
	goat.position.y = tiger.getPosition().y;


	/* start of game loop */
	while (!quit) {
		while (SDL_PollEvent(&event)) { // poll the event queue till it's empty	
			if (event.type == SDL_QUIT || input.keys.escape) {
				quit = true;
			}
		}

		frameStart = SDL_GetTicks();

		// get the keyboard state this frame
		input.updateKeyboard();
		// get the mouse state this frame
		input.updateMouse();

		/* clear the renderer to green, because it's a jungle */
		SDL_SetRenderDrawColor(ren, 0, 60, 0, 255);
		SDL_RenderClear(ren);

		// debug output
		if (input.pressed(MouseButton::left)) {
			std::cout
			<< "mouse.pos.x: " << input.mouse.x << std::endl
			<< "relative x: " << input.mouse.x / TILE_SIZE << std::endl
			<< "mouse.pos.y: " << input.mouse.y << std::endl
			<< "relative y: " << input.mouse.y / TILE_SIZE << std::endl
			<< "index: " << world.getRelative(input.mouse.x, input.mouse.y)
			<< std::endl;
		}

		if (input.mouse.left) {
			// a static variable to store if we are drawing walls or empty nodes
			static bool drawWall{ true };
			// test if the mouse was pressed over a wall or and empty nodes
			if ( input.pressed(MouseButton::left) ) {
				drawWall = (world.getRelative(input.mouse.x, input.mouse.y) != WALL);
			}
			// if the mouse is pressed over a wall, start deleting walls
			// if the mouse is pressed over an empty, start adding walls
			if ( drawWall ) {
				world.setRelative(input.mouse.x, input.mouse.y, WALL);
			} else {
				world.setRelative(input.mouse.x, input.mouse.y, EMPTY);
			}
		}
		// remove all the walls if enter is pressed
		if ( input.pressed(Key::enter) ) {
			world.clearWalls();
		}

		// place the goats with the right mouse
		if ( input.mouse.right ) {
			goat.setPosition(input.mouse.x / TILE_SIZE, input.mouse.y / TILE_SIZE);
		}

		// move the tiger with the middle mouse
		if ( input.mouse.middle ) {
			tiger.setPosition(input.mouse.x / TILE_SIZE, input.mouse.y / TILE_SIZE);
		}

		// remove all valuse from the grid except the walls
		world.clearPathingValues();
		// move the destination of the tiger according to the user
		tiger.setDestination(goat.position.x, goat.position.y);
		// generate a path from the tiger to the destination
		tiger.generatePath();
		// draw the world

		// toggle if paths are animated by pressing space
		static int drawPaths{ 0 };
		if (input.pressed(Key::space)) { drawPaths++; }
		// cycle between not drawing, drawing constantly, and animating the paths
		if ( drawPaths == 1 ) {
			world.drawPaths();
		} else if ( drawPaths == 2 ) {
			world.animatePaths();
		} else {
			drawPaths = 0;
		}	

		world.drawWalls();

		if ( tiger.hasFoundPath() ) { tiger.drawPath(); }
		// draw the tiger and goat
		tiger.drawPosition();
		goat.drawPosition();
		tiger.drawDestination();

		/* update the screen */
		SDL_RenderPresent(ren);

		// calculating frame rate
		frameEnd = SDL_GetTicks();
		frameTime = frameEnd - frameStart;
		//std::cout << "fps: " << 1000.f / frameTime << std::endl;

	} /* end of game loop */

	SDL_Quit();
	return 0;
}