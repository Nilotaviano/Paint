#include "CRectangle.h"

#include <SDL_opengl.h>
#include <GL\GLU.h>
#include <stdio.h>


CRectangle::CRectangle(float x, float y, float height, float width, unsigned char r, unsigned char g, unsigned char b, int rotation)
: x_(x), y_(y),
width_(width), height_(height),
r_(r), g_(g), b_(b),
rotation_(rotation),
IShape()
{
  border_rects_[0].x = -0.01;
  border_rects_[0].y = -0.01;
  border_rects_[0].width = 0.02f;
  border_rects_[0].height = 0.02f;

  border_rects_[1].x = -0.01;
  border_rects_[1].y = -0.01 + height_;
  border_rects_[1].width = 0.02f;
  border_rects_[1].height = 0.02f;

  border_rects_[2].x = -0.01 + width_;
  border_rects_[2].y = -0.01;
  border_rects_[2].width = 0.02f;
  border_rects_[2].height = 0.02f;

  border_rects_[3].x = -0.01 + width_;
  border_rects_[3].y = -0.01 + height_;
  border_rects_[3].width = 0.02f;
  border_rects_[3].height = 0.02f;
}


CRectangle::~CRectangle()
{

}

void CRectangle::Draw()
{
  glPushMatrix();

    glColor3ub(r_, g_, b_);

    glTranslatef(x_, y_, 0);
    glRectf(
      0,
      0,
      width_,
      height_
      );

    //Will draw 4 small gray squares on each corner of this rectangle
    if (selected) {
      glColor3ub(128, 128, 128);

      for (BorderRect rect : border_rects_) {
        glRectf(
          rect.x,
          rect.y,
          rect.x + rect.width,
          rect.y + rect.height
          );
      }
    }
  glPopMatrix();
}

void CRectangle::Update()
{

}

void CRectangle::Move(float mouse_x_offset, float mouse_y_offset)
{
  x_ += mouse_x_offset / 320;
  y_ += mouse_y_offset / 240;
}