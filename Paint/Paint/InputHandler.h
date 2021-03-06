#pragma once
#include <SDL.h>
#include <functional>
#include "IShape.h"


#define KEYBOARD_SIZE 256

class Paint;

class InputHandler
{
public:
  InputHandler(bool* quit, std::function<void()> resize_function, std::function<void(SDL_MouseButtonEvent)> handle_click_function);
	~InputHandler(void);
	void handleInput(SDL_Event event);
	bool isKeyPressed(SDL_Keycode key);
  bool isLMouseButtonPressed() { return l_mouse_button_; }
  void set_p_shape_(IShape* shape);
private:
  std::function<void()> p_resize_function_;
  std::function<void(SDL_MouseButtonEvent)> p_handle_click_function;
  bool* p_quit_;
	void handleKeyboard(SDL_KeyboardEvent kbEvent);
  void HandleMouseButton(SDL_MouseButtonEvent event);
  void HandleMouseMotion(SDL_MouseMotionEvent event);
	bool keys_[KEYBOARD_SIZE];
  bool l_mouse_button_;
  bool r_mouse_button_;

  IShape* p_shape_;
};
