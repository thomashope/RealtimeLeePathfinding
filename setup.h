#ifndef SETUP_H
#define SETUP_H
/* to get the project to build in visual studio do the following
first copy the .dll into the debug directory, where the .exe is located

then right click project and go to properties
- configuration properties > c/c++ > general > additional include directories
	add SDL2/include directory to the list

- configuration properties > linker > general > additional library directories
	SDL2/lib/x86 for the 32 bit library x64 for the 64 bit library

- configuration properties > linker > input > additional dependencies
	type in a line for the libraries you want to use, in this case
	SDL2.lib
	and
	SDL2main.lib

	check out "Let's Make An RPG (C++/SDL2) - Part 3 Continuation of Setting up"
	http://www.youtube.com/watch?v=wxtJYFp59Lg
*/

#include <SDL2/SDL.h>
#include <iostream>

const int SCREEN_WIDTH{ 640 };
const int SCREEN_HEIGHT{ 480 };

// sets up sdl
int initSDL();
// initialises the window handed to it
int initWindow(SDL_Window*& win, const char* name);
// initialises the renderer handed to it
int initRenderer(SDL_Window*& win, SDL_Renderer*& ren);

#endif
