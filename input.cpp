#include "input.h"

void Input::storeOldKeyboard() {
	oldKeys = keys;
}

void Input::storeOldMouse() {
	oldMouse = mouse;
}

bool Input::pressed(Key key) {
	switch (key)
	{
	case Key::escape:
		return keys.escape && !oldKeys.escape;
	case Key::space:
		return keys.space && !oldKeys.space;;
	case Key::up:
		return keys.up && !oldKeys.up;;
	case Key::down:
		return keys.down && !oldKeys.down;;
	case Key::left:
		return keys.left && !oldKeys.left;;
	case Key::right:
		return keys.right && !oldKeys.right;;
	case Key::enter:
		return keys.enter && !oldKeys.enter;;
	default:
		return false;
	}
}

bool Input::released(Key key) {
	switch (key)
	{
	case Key::escape:
		return oldKeys.escape && !keys.escape;
	case Key::space:
		return oldKeys.space && !keys.space;;
	case Key::up:
		return oldKeys.up && !keys.up;;
	case Key::down:
		return oldKeys.down && !keys.down;;
	case Key::left:
		return oldKeys.left && !keys.left;;
	case Key::right:
		return oldKeys.right && !keys.right;;
	case Key::enter:
		return oldKeys.enter && !keys.enter;;
	default:
		return false;
	}

}

bool Input::pressed(MouseButton button) {
	switch (button)
	{
	case MouseButton::left:
		return mouse.left && !oldMouse.left;
	case MouseButton::middle:
		return mouse.middle && !oldMouse.middle;
	case MouseButton::right:
		return mouse.right && !oldMouse.right;
	case MouseButton::scrollDown:
		return mouse.scrollDown && !oldMouse.scrollDown;
	case MouseButton::scrollUp:
		mouse.scrollUp && !oldMouse.scrollUp;
	default:
		return false;
	}
}

bool Input::released(MouseButton button) {
	switch (button)
	{
	case MouseButton::left:
		return !mouse.left && oldMouse.left;
	case MouseButton::middle:
		return !mouse.middle && oldMouse.middle;
	case MouseButton::right:
		return !mouse.right && oldMouse.right;
	case MouseButton::scrollDown:
		return !mouse.scrollDown && oldMouse.scrollDown;
	case MouseButton::scrollUp:
		return !mouse.scrollUp && oldMouse.scrollUp;
	default:
		return false;
	}
}

void Input::updateKeyboard() {
	// update the old keys to hold the keyboard state of the last frame
	storeOldKeyboard();
	// get the keyboard object
	const Uint8* keyState{ SDL_GetKeyboardState(nullptr) };
	// update the struct by testing againts the keyState
	keys.escape = (keyState[SDL_SCANCODE_ESCAPE] != 0);
	keys.space  = (keyState[SDL_SCANCODE_SPACE ] != 0);
	keys.up     = (keyState[SDL_SCANCODE_UP    ] != 0);
	keys.down   = (keyState[SDL_SCANCODE_DOWN  ] != 0);
	keys.left   = (keyState[SDL_SCANCODE_LEFT  ] != 0);
	keys.right  = (keyState[SDL_SCANCODE_RIGHT ] != 0);
	keys.enter  = (keyState[SDL_SCANCODE_RETURN] != 0);
}

void Input::updateMouse() {
	// update the old mouse to hold the mouse state last frame
	storeOldMouse();
	// update the x and y posiiton of the mouse struct
	// returns a bitmask of button presses which can be tested against using the SDL_ macros
	Uint32 mouseState{ SDL_GetMouseState(&mouse.x, &mouse.y) };
	mouse.left   = ((mouseState & SDL_BUTTON(SDL_BUTTON_LEFT  )) > 0 );
	mouse.middle = ((mouseState & SDL_BUTTON(SDL_BUTTON_MIDDLE)) > 0 );
	mouse.right  = ((mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT )) > 0 );
}