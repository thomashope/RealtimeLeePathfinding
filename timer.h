#ifndef TIMER_H
#define TIMER_H

#include <SDL2/SDL.h>
#include <iostream>
#include <string>
using namespace std;

class Timer {
	Uint32 startTime{ 0 };
	Uint32 timePassed{ 0 };
	string name;
public:
	Timer(string n) { name = n; }
	~Timer() {}

	inline void start()  { startTime = SDL_GetTicks(); }
	inline void reset()  { timePassed = startTime = 0; }
	inline void stop()   { timePassed = SDL_GetTicks(); timePassed -= startTime; }
	inline void output() { cout << name << ": " << timePassed / 1000.0f << endl; }
};

#endif
