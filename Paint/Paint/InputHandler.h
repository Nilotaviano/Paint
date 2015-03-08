#pragma once
#include <SDL.h>
#include <functional>


#define KEYBOARD_SIZE 256

class Paint;

class InputHandler
{
public:
  InputHandler(bool* quit, std::function<void()> resize_function);
	~InputHandler(void);
	void handleInput(SDL_Event event);
	bool isKeyPressed(SDL_Keycode key);
private:
  std::function<void()> p_resize_function_;
  bool* p_quit_;
	void handleKeyboard(SDL_KeyboardEvent kbEvent);
  void HandleMouseButton(SDL_MouseButtonEvent event);
  void HandleMouseMotion(SDL_MouseMotionEvent event);
	bool keys_[KEYBOARD_SIZE];
  bool l_mouse_button_;
};
