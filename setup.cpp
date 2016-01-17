#include "setup.h"

int initSDL() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		SDL_Delay(1000);
		return 1;
	}
	return 0;
}

int initWindow(SDL_Window* &win, const char* name) {
	win = SDL_CreateWindow(name, 100, 100, 640, 480, SDL_WINDOW_SHOWN);
	if (win == nullptr) {
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Delay(1000);
		SDL_Quit();
		return 1;
	}
	return 0;
}

int initRenderer(SDL_Window* &win, SDL_Renderer*& ren) {
	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	// turn on alpha blending
	SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
	if (ren == nullptr) {
		SDL_DestroyWindow(win);
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		SDL_Delay(1000);
		SDL_Quit();
		return 1;
	}
	return 0;
}