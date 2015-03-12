#include "CRectangle.h"

#include <SDL_opengl.h>
#include <GL\GLU.h>
#include <math.h>
#include <stdio.h>


CRectangle::CRectangle(float x, float y, float height, float width, unsigned char r, unsigned char g, unsigned char b, float rotation)
: x_(x), y_(y),
width_(width), height_(height),
r_(r), g_(g), b_(b),
rotation_(rotation),
IShape()
{
  set_border_rects_();
}

void CRectangle::set_border_rects_() {
  //Sides
  //Left
  border_rects_[0].x = -0.01;
  border_rects_[0].y = height_ / 2 - 0.01;
  border_rects_[0].position = BorderRectPosition::LEFT;
  //Right
  border_rects_[1].x = width_ - 0.01;
  border_rects_[1].y = height_ / 2 - 0.01;
  border_rects_[1].position = BorderRectPosition::RIGHT;
  //Bottom
  border_rects_[2].x = width_ / 2 - 0.01;
  border_rects_[2].y = -0.01;
  border_rects_[2].position = BorderRectPosition::BOTTOM;
  //Up
  border_rects_[3].x = width_ / 2 - 0.01;
  border_rects_[3].y = height_ + -0.01;
  border_rects_[3].position = BorderRectPosition::TOP;

  //Corners
  //Bottom left
  border_rects_[4].x = -0.01;
  border_rects_[4].y = -0.01;
  border_rects_[4].position = BorderRectPosition::BOTTOM_LEFT;
  //Top left
  border_rects_[5].x = -0.01;
  border_rects_[5].y = -0.01 + height_;
  border_rects_[5].position = BorderRectPosition::TOP_LEFT;
  //Bottom right
  border_rects_[6].x = -0.01 + width_;
  border_rects_[6].y = -0.01;
  border_rects_[6].position = BorderRectPosition::BOTTOM_RIGHT;
  //Top right
  border_rects_[7].x = -0.01 + width_;
  border_rects_[7].y = -0.01 + height_;
  border_rects_[7].position = BorderRectPosition::TOP_RIGHT;
}

CRectangle::~CRectangle()
{

}

void CRectangle::Draw()
{
  glPushMatrix();

  glColor3ub(r_, g_, b_);

  glRotatef(rotation_, width_ / 2, height_ / 2, 0);

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

bool CRectangle::IsMouseOver(float mouse_x, float mouse_y)
{
  //Convert mouse coordinates to "my world" coordinates
  mouse_x = (mouse_x - 320) / 320;
  mouse_y = -(mouse_y - 480);
  mouse_y = (mouse_y - 240) / 240;

  if (selected) {
    for (BorderRect &rect : border_rects_) {
      if (mouse_x > x_ + rect.x && mouse_x < x_ + rect.x + rect.width &&
        mouse_y > y_ + rect.y && mouse_y < y_ + rect.y + rect.height)
      {
        rect.selected = true;
        return true;
      }
      else {
        rect.selected = false;
      }
    }
  }

  if (mouse_x > x_ && mouse_x < x_ + width_ &&
    mouse_y > y_ && mouse_y < y_ + height_)
  {
    return true;
  }
  else {
    return false;
  }
}

void CRectangle::ReceiveMouseMotion(float mouse_x_offset, float mouse_y_offset)
{
  for (BorderRect rect : border_rects_) {
    if (rect.selected)
    {
      Resize(mouse_x_offset, mouse_y_offset, rect.position);
      return;
    }
  }

  //If none of the border rects are selected, it means that the this CRectangle itself is selected, therefore it moves.
  Move(mouse_x_offset, mouse_y_offset);
}

void CRectangle::Move(float mouse_x_offset, float mouse_y_offset)
{
  x_ += mouse_x_offset / 320;
  y_ += mouse_y_offset / 240;
}

void CRectangle::Resize(float mouse_x_offset, float mouse_y_offset, BorderRectPosition position)
{
  mouse_x_offset = mouse_x_offset / 320;
  mouse_y_offset = mouse_y_offset / 240;

  switch (position) {

  case BorderRectPosition::LEFT:
    x_ += mouse_x_offset;
    width_ -= mouse_x_offset;
    break;

  case BorderRectPosition::RIGHT:
    width_ += mouse_x_offset;
    break;

  case BorderRectPosition::TOP:
    height_ += mouse_y_offset;
    break;

  case BorderRectPosition::BOTTOM:
    y_ += mouse_y_offset;
    height_ -= mouse_y_offset;
    break;

  case BorderRectPosition::BOTTOM_LEFT:
    if (abs(mouse_x_offset) >= abs(mouse_y_offset)) {
      x_ += mouse_x_offset;
      width_ -= mouse_x_offset;
      y_ += mouse_x_offset;
      height_ -= mouse_x_offset;
    }
    else {
      x_ += mouse_y_offset;
      width_ -= mouse_y_offset;
      y_ += mouse_y_offset;
      height_ -= mouse_y_offset;
    }
    break;

  case BorderRectPosition::BOTTOM_RIGHT:
    if (abs(mouse_x_offset) >= abs(mouse_y_offset)) {
      width_ += mouse_x_offset;
      y_ -= mouse_x_offset;
      height_ += mouse_x_offset;
    }
    else {
      width_ -= mouse_y_offset;
      y_ += mouse_y_offset;
      height_ -= mouse_y_offset;
    }
    break;

  case BorderRectPosition::TOP_LEFT:
    if (abs(mouse_x_offset) >= abs(mouse_y_offset)) {
      x_ += mouse_x_offset;
      width_ -= mouse_x_offset;
      height_ -= mouse_x_offset;
    }
    else {
      x_ -= mouse_y_offset;
      width_ += mouse_y_offset;
      height_ += mouse_y_offset;
    }
    break;

  case BorderRectPosition::TOP_RIGHT:
    if (abs(mouse_x_offset) >= abs(mouse_y_offset)) {
      width_ += mouse_x_offset;
      height_ += mouse_x_offset;
    }
    else {
      width_ += mouse_y_offset;
      height_ += mouse_y_offset;
    }
    break;
  }

  set_border_rects_();
}

void CRectangle::Rotate(float mouse_x_offset, float mouse_y_offset)
{
  mouse_x_offset = mouse_x_offset / 320;
  mouse_y_offset = mouse_y_offset / 240;
  
  rotation_ += mouse_x_offset * 360;
}