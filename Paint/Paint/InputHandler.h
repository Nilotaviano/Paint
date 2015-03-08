#pragma once
#include <SDL.h>
#include <functional>
#include "IShape.h"


#define KEYBOARD_SIZE 256

class Paint;

class InputHandler
{
public:
  InputHandler(bool* quit, std::function<void()> resize_function, std::function<void(int, int)> handle_click_function);
	~InputHandler(void);
	void handleInput(SDL_Event event);
	bool isKeyPressed(SDL_Keycode key);
  bool isLMouseButtonPressed() { return l_mouse_button_; }
  void set_shape(IShape* shape) { shape_ = shape; }
private:
  std::function<void()> p_resize_function_;
  std::function<void(int, int)> p_handle_click_function;
  bool* p_quit_;
	void handleKeyboard(SDL_KeyboardEvent kbEvent);
  void HandleMouseButton(SDL_MouseButtonEvent event);
  void HandleMouseMotion(SDL_MouseMotionEvent event);
	bool keys_[KEYBOARD_SIZE];
  bool l_mouse_button_;

  IShape* shape_;
};
