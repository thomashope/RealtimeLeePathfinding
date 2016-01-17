#include "tiger.h"
#include "board.h"
#include "utilities.h"

#include <array>

Tiger::Tiger(Board* map, int x, int y) {
	// take a pointer to the world
	world = map;
	// setup the tiger
	position.x = x;
	position.y = y;
	position.i = START;
	// set the destination to the tigers current location
	destination = position;
	destination.i = END;
	std::cout << "There is a tiger in the jungle." << std::endl;
}

Tiger::~Tiger() {
	std::cout << "The tiger died." << std::endl;
}

void Tiger::generatePath() {
	world->set(position);
	world->set(destination);

	generateDistances();

	if (pathFound) { traceBack(); }
}

void Tiger::generateDistances() {
	workingSet.push_front(position);

	// loop while there are nodes in the working set
	// and the end has not been reached
	while ( !workingSet.empty() ) {
		pathFound = checkAdjIndex(workingSet.front(), END);
		if (pathFound) { break; }

		// update the point at the front of the working set
		updateAdjNodes( workingSet.front() );
		// remove the updated point from the set
		workingSet.pop_front();

	}
	// after the path has been found, empty the working set
	workingSet.erase( workingSet.begin(), workingSet.end() );
}

bool Tiger::checkAdjIndex(Node current, Uint16 index) {
	// check it itself is not the index
	if (world->get(current.x, current.y) == index) { return true; }
	// then check the adjacent index
	// making sure they don't wrap around the board
	if (current.x != 0) { if (world->get(current.x - 1, current.y) == index) { return true; } }
	if (current.y != 0) { if (world->get(current.x, current.y - 1) == index) { return true; } }
	if (world->get(current.x + 1, current.y) == index) { return true; }
	if (world->get(current.x, current.y + 1) == index) { return true; }
	return false;
}

void Tiger::updateAdjNodes(const Node& current) {

	// if you see a red line under 'Node' below is a bug in intelisense https://connect.microsoft.com/VisualStudio/feedback/details/865188/c-misdiagnosed-error-intellisense-braces-cannot-be-omitted-for-this-subobject-initializer

	// traceback searches diagonals but updateAdjNodes doesn't update diagonals
	// this can produces non optimal paths
	std::array<Node, 4> adjNodes{
		Node( current.x - 1, current.y ), // west
		Node( current.x, current.y - 1 ), // north
		Node( current.x + 1, current.y ), // east
		Node( current.x, current.y + 1 ), // south
	};

	/*
	// This version updates the corners too, but updating has some wired results,
	// could do with a visualisation to debug and modified traceback to get working?
	std::array<Node, 8> adjNodes{
		Node{ current.x - 1, current.y }, // west
		Node{ current.x, current.y - 1 }, // north
		Node{ current.x + 1, current.y }, // east
		Node{ current.x, current.y + 1 }, // south
		Node{ current.x - 1, current.y - 1 }, // north west
		Node{ current.x + 1, current.y - 1 }, // north east
		Node{ current.x + 1, current.y + 1 }, // south east
		Node{ current.x - 1, current.y + 1 }  // south west
	};*/

	for (auto& node : adjNodes) {
		if (node.x < GRID_WIDTH && node.y < GRID_HEIGHT) {
			world->assignIndexTo(node);
			if (node.i == EMPTY) {
				node.i = current.i + 1;
				world->set(node);
				workingSet.push_back(node);
			}
		}
	}
}

/*
void Tiger::updateAdjNodes(const Node& current) {
	// create a Node for each new direction
	Node west{ current.x - 1, current.y };
	// if it's empty, set it to newIndex and update the board
	if (west.x < GRID_WIDTH && west.y < GRID_HEIGHT) { // test that the Node is not out of bounds
		// set the index of west to the index it's corresponding grid on the board
		world->assignIndexTo(west);
		if (west.i == EMPTY) {
			west.i = current.i + 1;
			world->set(west);
			workingSet.push_back(west);
		}
	}

	// do the same for the north, east and south Nodes
	Node north{ current.x, current.y - 1 };
	if (north.x < GRID_WIDTH && north.y < GRID_HEIGHT) { // test that the Node is not out of bounds
		world->assignIndexTo(north);
		if (north.i == EMPTY) {
			north.i = current.i + 1;
			world->set(north);
			workingSet.push_back(north);
		}
	}

	Node east{ current.x + 1, current.y };
	if (east.x < GRID_WIDTH && east.y < GRID_HEIGHT) { // test that the Node is not out of bounds
		world->assignIndexTo(east);
		if (east.i == EMPTY) {
			east.i = current.i + 1;
			world->set(east);
			workingSet.push_back(east);
		}
	}

	Node south{ current.x, current.y + 1 };
	if (south.x < GRID_WIDTH && south.y < GRID_HEIGHT) { // test that the Node is not out of bounds
		world->assignIndexTo(south);
		if (south.i == EMPTY) {
			south.i = current.i + 1;
			world->set(south);
			workingSet.push_back(south);
		}
	}
}
*/

void Tiger::traceBack() {
	// first make sure the stack is empty
	while (!path.empty()) {
		path.pop();
	}
	// push the destination of the tiger to the stack
	path.push(destination);
	// exit imideiately if the tiger is already at it's destination
	if (destination.x == position.x && destination.y == position.y) { return; }

	// nodeTraceBack is the old version
	// I tried cleaning it up using vectors and iterators. The vector version has less lines but is less preformant
	// clarity between the two version is subjective (personally i prefer the vector version)

	this->vectorTraceBack();
	//this->nodeTraceBack();

	pathFound = ( path.top().i == START );
}

void Tiger::vectorTraceBack() {
	// a predicate for removing nodes that lie off the grid

	struct isOutOfBounds {
		bool operator () (const Node& node) {
			return ((node.x > GRID_WIDTH) || (node.y > GRID_HEIGHT));
		}
	};

	do {
		Uint16 currentX = path.top().x;
		Uint16 currentY = path.top().y;
		// initialise the vector with the points of the compass
		std::vector<Node> adjNodes{
			Node( currentX - 1, currentY ),     // west
			Node( currentX - 1, currentY - 1 ), // north west
			Node( currentX, currentY - 1 ),     // north
			Node( currentX + 1, currentY - 1 ), // north east
			Node( currentX + 1, currentY ),     // east
			Node( currentX + 1, currentY + 1 ), // south east
			Node( currentX, currentY + 1 ),     // south
			Node( currentX - 1, currentY + 1 ), // south west
		};
		// remove all Nodes that lie outside the board
		adjNodes.erase(std::remove_if(adjNodes.begin(), adjNodes.end(), isOutOfBounds()), adjNodes.end());
		// get the indexes from the board
		for (auto& node : adjNodes) { world->assignIndexTo(node); }
		// remove nodes that are skiping over wall corners diagonaly
		for (auto node = adjNodes.begin(); node != adjNodes.end();) {
			if (skipsWallDiagonaly(*node))  {
				node = adjNodes.erase(node);
			} else {
				node++;
			}
		}
		// check if the start node is present
		for (const auto& node : adjNodes) {
			if (node.i == START) {
				path.push(node);
				break;
			}
		}
		// find the Node with the lowest index
		// assign if lower and valid ensures Nodes are not out of bounds
		Node lowest = path.top();
		for (const auto& node : adjNodes) { assignIfLowerAndWithinBounds(lowest, node); }

		path.push(lowest);
	} while (path.top().i != START && path.size() < MAX_PATH_SIZE);
}

void Tiger::nodeTraceBack() {
	// decalre the points to represent the north, south, east, west and diagonals
	Node west, northWest, north, northEast, east, southEast, south, southWest;

	do {
		// initialise the points
		west.x      = path.top().x - 1;	     west.y = path.top().y;
		northWest.x = path.top().x - 1; northWest.y = path.top().y - 1;
		north.x     = path.top().x;	        north.y = path.top().y - 1;
		northEast.x = path.top().x + 1; northEast.y = path.top().y - 1;
		east.x      = path.top().x + 1;	     east.y = path.top().y;
		southEast.x = path.top().x + 1; southEast.y = path.top().y + 1;
		south.x     = path.top().x;	        south.y = path.top().y + 1;
		southWest.x = path.top().x - 1; southWest.y = path.top().y + 1;

		// get the indexes of the adjacent points from the bord
		world->assignIndexTo(     west);
		world->assignIndexTo(northWest);
		world->assignIndexTo(    north);
		world->assignIndexTo(northEast);
		world->assignIndexTo(     east);
		world->assignIndexTo(southEast);
		world->assignIndexTo(    south);
		world->assignIndexTo(southWest);

		// check if any of the adjacent Nodes are the start
		if (     west.i == START) { path.push(     west); break; }
		if (    north.i == START) { path.push(    north); break; }
		if (     east.i == START) { path.push(     east); break; }
		if (    south.i == START) { path.push(    south); break; }
		// making sure the path can't skip through walls diagonaly
		if (northWest.i == START && west.i != WALL && north.i != WALL) { path.push(northWest); break; }
		if (northEast.i == START && east.i != WALL && north.i != WALL) { path.push(northEast); break; }
		if (southEast.i == START && east.i != WALL && south.i != WALL) { path.push(southEast); break; }
		if (southWest.i == START && west.i != WALL && south.i != WALL) { path.push(southWest); break; }

		// find the Node with the lowest index
		// assign if lowe and valid ensures Nodes are not out of bounds
		Node lowest = path.top();

		assignIfLowerAndWithinBounds(lowest, west);
		assignIfLowerAndWithinBounds(lowest, north);
		assignIfLowerAndWithinBounds(lowest, east);
		assignIfLowerAndWithinBounds(lowest, south);
		// don't let the tiger skip corners diagonaly
		if (north.i != WALL && west.i != WALL) { assignIfLowerAndWithinBounds(lowest, northWest); }
		if (north.i != WALL && east.i != WALL) { assignIfLowerAndWithinBounds(lowest, northEast); }
		if (south.i != WALL && east.i != WALL) { assignIfLowerAndWithinBounds(lowest, southEast); }
		if (south.i != WALL && west.i != WALL) { assignIfLowerAndWithinBounds(lowest, southWest); }
		// push the lowest element to the path
		path.push(lowest);

		// keep looping untill the top element is the start.
	} while (path.top().i != START && path.size() < MAX_PATH_SIZE);
}

void Tiger::drawPath() {
	while (!path.empty()) {
		SDL_SetRenderDrawColor(ren, 180, 180, 200, 200);
		world->drawNode(path.top());
		path.pop();
	}
}

void Tiger::drawDestination() {
	SDL_Rect target{ destination.x * TILE_SIZE, destination.y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
	SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
	SDL_RenderDrawRect(ren, &target);
}

void Tiger::drawPosition() {
	SDL_Rect pos{ position.x * TILE_SIZE, position.y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
	SDL_SetRenderDrawColor(ren, 255, 166, 0, 255);
	SDL_RenderFillRect(ren, &pos);
}

void Tiger::setDestination(int x, int y)  {
	clamp(0, GRID_WIDTH,  x);
	clamp(0, GRID_HEIGHT, y);
	destination.x = x;
	destination.y = y;
}

void Tiger::setPosition(int x, int y) {
	clamp(0, GRID_WIDTH,  x);
	clamp(0, GRID_HEIGHT, y);
	position.x = x;
	position.y = y;
}
