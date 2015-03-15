#pragma once
#include <SDL.h>
#include <functional>

#include "IButton.h"

class TextButton
  : public IButton
{
public:
  TextButton(float x, float y, float width, float height, int r, int g, int b, char* text, int font_size, std::function<void()> callback_function);
	~TextButton();
	void Draw();
  bool IsMouseOver(float mouse_x, float mouse_y);
  void HandleClick();
	bool selected;
private:
	float x_;
	float y_;
	float width_;
	float height_;
	SDL_Color color_;
	SDL_Color textColor_;
  std::function<void()> callback_function_;
};