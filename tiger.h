#ifndef TIGER_H
#define TIGER_H

#include <SDL2/SDL.h>
#include <stack>
#include <deque>
#include <vector>
#include <array>
#include "setup.h"
#include "board.h"
#include "timer.h"

const Uint16 MAX_PATH_SIZE{ 512 };

/* Tigers are effective pathfinders */
class Tiger {
public:
	// construct with a start location and knowledge of the world
	Tiger(Board* map, int x, int y);
	// euthanise
	~Tiger();

	// finds a path between the the tiger and it's target and pushes it to the stack
	void generatePath();
	// draws the path from the position to the destination
	void drawPath();
	// draw a square to represent the tiger
	void drawPosition();
	// highlight the tigers target square
	void drawDestination();
	// chooses a Node which the tiger should try and get too if there are multiple targets
	void selectTarget(std::vector<Node>& targets);

	// getters and setters
	void setPosition(int x, int y);
	Node getPosition() { return position; }
	void setDestination(int x, int y);
	Node getDestination() { return destination; }
	bool hasFoundPath() { return pathFound; }

private:
	// the world in which the tiger lives
	Board* world;
	// the location of the tiger
	Node position;
	// where the tiger wants to go
	Node destination;
	// true if the tiger found a path to it's destination
	bool pathFound{ false };
	// holds the path of Nodes fom the start point to the target point
	std::stack<Node> path;
	// holds a queue of all the Nodes that have yet to be checked
	std::deque<Node> workingSet;
	// marks the grid with distance values for pathfinding
	void generateDistances();
	// finds a route by walking back from the destination
	void traceBack();
	void vectorTraceBack();
	void nodeTraceBack();
	// returns true if the passed index is adjacent to the passed point
	bool checkAdjIndex(Node current, Uint16 index);
	// sets unmarked points to the passed points value + 1
	void updateAdjNodes(const Node& current);

	inline void assignIfLowerAndWithinBounds(Node& champion, const Node& competetor) {
		// check the competetor is within the grid bounds
		if (competetor.x < GRID_WIDTH && competetor.y < GRID_HEIGHT) {
			// if the competetor's index is less than the champions, the champion is the competetor
			if (competetor.i < champion.i) { champion = competetor; }
		}
	}

	// used as function predicates in traceback
	inline bool skipsWallDiagonaly(const Node& node) {
		return (world->get(node.x, path.top().y) == WALL || world->get(path.top().x, node.y) == WALL);
	}

};



#endif
