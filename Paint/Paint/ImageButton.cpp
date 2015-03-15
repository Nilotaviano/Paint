#include "ImageButton.h"
#include <SDL_opengl.h>

ImageButton::ImageButton(float x, float y, float width, float height, int r, int g, int b, std::function<void()> callback_function)
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

}


ImageButton::~ImageButton()
{
}

void ImageButton::Draw() {
  if (selected) {
    glColor3ub(0, 0, 150);
    glLineWidth(4);
    
    glBegin(GL_LINE_LOOP);
      glVertex2f(x_, y_);
      glVertex2f(x_ + width_, y_);
      glVertex2f(x_ + width_, y_ + height_);
      glVertex2f(x_, y_ + height_);
    glEnd();
  }

  glColor3ub(color_.r, color_.g, color_.b);
  glRectf(
    x_,
    y_,
    x_ + width_,
    y_ + height_);

}

bool ImageButton::IsMouseOver(float mouse_x, float mouse_y)
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

void ImageButton::HandleClick() {
  callback_function_();
}