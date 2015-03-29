#pragma once
#include <SDL.h>
#include <functional>

#include "IButton.h"

class ImageButton :
  public IButton
{
public:
  ImageButton(float x, float y, float width, float height, int r, int g, int b, std::function<void()> callback_function, int shape);
  ~ImageButton();
  void Draw();
  bool IsMouseOver(float mouse_x, float mouse_y);
  void HandleClick();
  bool selected;
private:

  int shape_;
  float x_;
  float y_;
  float width_;
  float height_;
  SDL_Color color_;
  std::function<void()> callback_function_;
};
