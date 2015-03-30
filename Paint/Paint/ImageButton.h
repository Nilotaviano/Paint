#pragma once
#include <SDL.h>
#include <functional>
#include <list>

#include "IButton.h"

class ImageButton :
  public IButton
{
public:
  ImageButton(float x, float y, float width, float height, std::function<void(SDL_Color)> callback_function, int sides, bool is_parent);
  ImageButton(float x, float y, float width, float height, int r, int g, int b, std::function<void(SDL_Color)> callback_function, int shape, bool is_parent);
  ~ImageButton();
  void Draw();
  bool IsMouseOver(float mouse_x, float mouse_y);
  void HandleClick();
  bool is_selected_;
private:
  std::list<ImageButton> buttons_;

  int num_sides_;
  bool is_parent_;
  float x_;
  float y_;
  float width_;
  float height_;
  SDL_Color color_;
  std::function<void(SDL_Color)> callback_function_;
};
