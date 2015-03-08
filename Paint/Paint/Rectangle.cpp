#include "Rectangle.h"

#include <SDL_opengl.h>
#include <GL\GLU.h>


Rectangle::Rectangle(float x, float y, float height, float width, int rotation = 0)
: x_(x), y_(y), 
width_(width), height_(height), 
rotation_(rotation),
selected(false)
{
}


Rectangle::~Rectangle()
{

}

void Rectangle::Draw()
{
  glColor3f(1, 0, 0);

  glRectf(
    x_,
    y_,
    x_ + width_,
    y_ + height_
    );
}

void Rectangle::Update()
{

}

void Rectangle::Move(float x_offset, float y_offset)
{
  x_ += x_offset;
  y_ += y_offset;
}