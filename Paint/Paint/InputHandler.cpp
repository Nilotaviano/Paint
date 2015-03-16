#include "InputHandler.h"
#include <stdio.h>

#include "Paint.h"


InputHandler::InputHandler(bool* quit, std::function<void()> resize_function, std::function<void(SDL_MouseButtonEvent)> handle_click_function)
: p_quit_(quit),
p_resize_function_(resize_function),
p_handle_click_function(handle_click_function),
l_mouse_button_(false),
r_mouse_button_(false),
shape_(nullptr)
{
  for (int i = 0; i < KEYBOARD_SIZE; i++) {
    keys_[i] = false;
  }
}

InputHandler::~InputHandler(void)
{
}

void InputHandler::handleInput(SDL_Event event)
{
  switch (event.type)
  {
  case SDL_KEYUP:
  case SDL_KEYDOWN:
    handleKeyboard(event.key);
    break;
  case SDL_MOUSEBUTTONDOWN:
  case SDL_MOUSEBUTTONUP:
    HandleMouseButton(event.button);
    break;
  case SDL_MOUSEMOTION:
    HandleMouseMotion(event.motion);
    break;
  case SDL_WINDOWEVENT:
    if (event.window.event == SDL_WINDOWEVENT_RESIZED && p_resize_function_ != nullptr) {
      p_resize_function_();
    }
    break;
  case SDL_QUIT:
    *p_quit_ = true;
    break;
  default:
    printf("Unhandled Event: %i\n", event.type);
    break;
  }
}

void InputHandler::handleKeyboard(SDL_KeyboardEvent kbEvent)
{
  int key = kbEvent.keysym.sym;

  if (key < KEYBOARD_SIZE) {
    switch (kbEvent.type){
    case SDL_KEYUP:
      keys_[kbEvent.keysym.sym] = false;
      break;
    case SDL_KEYDOWN:
      keys_[kbEvent.keysym.sym] = true;
      break;
    }
  }
}

void InputHandler::HandleMouseButton(SDL_MouseButtonEvent event) {
  switch (event.type) {
  case SDL_MOUSEBUTTONDOWN:
    p_handle_click_function(event);
    if (event.button == SDL_BUTTON_LEFT) {
      l_mouse_button_ = true;
    }
    else if (event.button == SDL_BUTTON_RIGHT) {
      r_mouse_button_ = true;
    }
    break;
  case SDL_MOUSEBUTTONUP:
    if (event.button == SDL_BUTTON_LEFT) {
      shape_ = nullptr;
      l_mouse_button_ = false;
    }
    else if (event.button == SDL_BUTTON_RIGHT) {
      r_mouse_button_ = false;
    }
    break;
  }
}

void InputHandler::HandleMouseMotion(SDL_MouseMotionEvent event) {
  if (l_mouse_button_ && shape_ != nullptr) {
    shape_->ReceiveMouseMotion(event.xrel, -event.yrel);  
  }
}

bool InputHandler::isKeyPressed(SDL_Keycode key)
{
  if (key < KEYBOARD_SIZE) {
    return keys_[key];
  }
  else {
    return false;
  }
}