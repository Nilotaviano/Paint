#include "TextButton.h"
#include <SDL_opengl.h>


TextButton::TextButton(float x, float y, float width, float height, int r, int g, int b, char* text, int font_size, std::function<void()> callback_function)
: callback_function_(callback_function)
{
	selected = false;
	x_ = x;
	y_ = y;
	width_ = width;
	height_ = height;

	color_.r = r;
	color_.g = g;
	color_.b = b;

	textColor_.r = 255;
	textColor_.g = 255;
	textColor_.b = 255;
}

TextButton::~TextButton()
{
}

void TextButton::Draw() {
	float outLineOffSet = 0.02f;
	if (selected) {
		glColor3ub(0, 0, 255);
		glRectf(
			x_ - outLineOffSet,
			y_ - outLineOffSet,
			x_ + width_ + outLineOffSet,
			y_ + height_ + outLineOffSet);
	}

  glColor3ub(color_.r, color_.g, color_.b);
	glRectf(
		x_,
		y_,
		x_ + width_,
		y_ + height_);

}

bool TextButton::IsMouseOver(float mouse_x, float mouse_y)
{
  //Convert mouse coordinates to "my world" coordinates
  mouse_x = (mouse_x - 320) / 320;
  mouse_y = -(mouse_y - 480);
  mouse_y = (mouse_y - 240) / 240;

  if (mouse_x > x_ && mouse_x < x_ + width_ &&
    mouse_y > y_ && mouse_y < y_ + height_)
  {
    return true;
  }
  else {
    return false;
  }
}

void TextButton::HandleClick() {
  callback_function_();
}