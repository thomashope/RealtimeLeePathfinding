#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>

enum class Key { escape, space, up, down, left, right, enter };
enum class MouseButton { left, middle, right, scrollDown, scrollUp };

class Input {
private:
	// another sturct to store mouse information
	struct Mouse{
		int x, y;
		bool left{ false };
		bool middle{ false };
		bool right{ false };
		bool scrollDown{ false };
		bool scrollUp{ false };
	};
	// a struct to hold they keys we care about
	struct Keyboard{
		bool escape{ false };
		bool space{ false };
		bool up{ false };
		bool down{ false };
		bool left{ false };
		bool right{ false };
		bool enter{ false };
	};

	// copies the input from last frame so we can test for key presses and releases
	void storeOldKeyboard();
	void storeOldMouse();

public:

	Mouse mouse, oldMouse;
	Keyboard keys, oldKeys;

	bool pressed(Key key);
	bool released(Key key);
	bool pressed(MouseButton button);
	bool released(MouseButton button);



	// gets the latest input since pollEvents was called
	void updateKeyboard();
	void updateMouse();
};

#endif
