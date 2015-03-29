#include "CRectangle.h"

#include <SDL_opengl.h>
#include <GL\GLU.h>
#include <math.h>
#include <stdio.h>


CRectangle::CRectangle(float x, float y, float height, float width, unsigned char r, unsigned char g, unsigned char b, float angle)
: x_(x), y_(y),
width_(width), height_(height),
r_(r), g_(g), b_(b),
angle_(angle),
x_shear_(0), y_shear_(0),
IShape()
{
  x_radius_ = width_ / 2;
  y_radius_ = height_ / 2;
  set_border_rects_();
  set_shear_rects();

  for (BorderRect &rect : border_rects_) {
    rect.height = 0.02f;
    rect.width = 0.02f;
    rect.selected = false;
  }

  shear_rects[0].height = 0.02f;
  shear_rects[0].width = 0.04f;
  shear_rects[0].selected = false;
  shear_rects[1].height = 0.04f;
  shear_rects[1].width = 0.02f;
  shear_rects[1].selected = false;
}

void CRectangle::set_border_rects_() {
  //Sides
  //Left
  border_rects_[0].x = -x_radius_ - 0.01f;
  border_rects_[0].y = -0.01f;
  border_rects_[0].position = BorderRectPosition::LEFT;
  //Right
  border_rects_[1].x = x_radius_ - 0.01f;
  border_rects_[1].y = -0.01f;
  border_rects_[1].position = BorderRectPosition::RIGHT;
  //Bottom
  border_rects_[2].x = -0.01f;
  border_rects_[2].y = -y_radius_ - 0.01f;
  border_rects_[2].position = BorderRectPosition::BOTTOM;
  //Up
  border_rects_[3].x = -0.01f;
  border_rects_[3].y = y_radius_ - 0.01f;
  border_rects_[3].position = BorderRectPosition::TOP;

  //Corners
  //Bottom left
  border_rects_[4].x = -x_radius_ - 0.01f;
  border_rects_[4].y = -y_radius_ - 0.01;
  border_rects_[4].position = BorderRectPosition::BOTTOM_LEFT;
  //Top left
  border_rects_[5].x = -x_radius_ - 0.01f;
  border_rects_[5].y = y_radius_ - 0.01;
  border_rects_[5].position = BorderRectPosition::TOP_LEFT;
  //Bottom right
  border_rects_[6].x = x_radius_ - 0.01f;
  border_rects_[6].y = -y_radius_ - 0.01;
  border_rects_[6].position = BorderRectPosition::BOTTOM_RIGHT;
  //Top right
  border_rects_[7].x = x_radius_ - 0.01f;
  border_rects_[7].y = y_radius_ - 0.01;
  border_rects_[7].position = BorderRectPosition::TOP_RIGHT;
}

void CRectangle::set_shear_rects() {
  //Above the shape
  shear_rects[0].x = -.02f;
  shear_rects[0].y = y_radius_ + .02f;
  shear_rects[0].position = BorderRectPosition::TOP;
  //Right side of the shape
  shear_rects[1].x = x_radius_ + .02f;
  shear_rects[1].y = -.02f;
  shear_rects[1].position = BorderRectPosition::RIGHT;
}

CRectangle::~CRectangle()
{

}

void CRectangle::Draw()
{
  glPushMatrix();
  glColor3ub(r_, g_, b_);

  glTranslatef(x_, y_, 0);

  glRotatef(angle_, 0, 0, 1.0f);

  shearMatrix(x_shear_, y_shear_);

  glScalef(width_, height_, 1);

  glRectf(
    -0.5f,
    -0.5f,
    0.5f,
    0.5f
    );

  glLineWidth(2.5);
  glColor3f(0.0f, 0.0f, 0.0f);

  glBegin(GL_LINES);
  glVertex2f(-0.5f, -0.5f);
  glVertex2f(0.5f, -0.5f);

  glVertex2f(0.5f, -0.5f);
  glVertex2f(0.5f, 0.5f);

  glVertex2f(0.5f, 0.5f);
  glVertex2f(-0.5f, 0.5f);

  glVertex2f(-0.5f, 0.5f);
  glVertex2f(-0.5f, -0.5f);
  glEnd();


  glLoadIdentity();

  glTranslatef(x_, y_, 0);

  //Center on origin before rotating
  glRotatef(angle_, 0, 0, 1.0f);

  shearMatrix(x_shear_, y_shear_);

  //Will Draw 4 small gray squares on each corner of this rectangle
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

    for (BorderRect rect : shear_rects) {
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

bool CRectangle::IsMouseOver(float mouse_x, float mouse_y)
{
  float click_focus;
  int screenWidth;
  int screenHeight;
  int vPort[4];
  float p[2];

  glGetIntegerv(GL_VIEWPORT, vPort);
  screenWidth = vPort[2];
  screenHeight = vPort[3];

  //Convert mouse coordinates to "my world" coordinates
  mouse_x = (mouse_x - (screenWidth / 2)) / (screenWidth / 2);
  mouse_y = -(mouse_y - screenHeight);
  mouse_y = (mouse_y - (screenHeight / 2)) / (screenHeight / 2);

  if (angle_ != 0) {
    mouse_x -= x_;
    mouse_y -= y_;
    //Negative angle for undoing the rotation
    rotatePoint(mouse_x, mouse_y, -angle_, p);
    mouse_x = p[0] + x_;
    mouse_y = p[1] + y_;
  }

  mouse_x -= x_;
  mouse_y -= y_;
  //Negative x_shear_ and y_shear_ for unshearing the mouse click point
  shearPoint(mouse_x, mouse_y, -x_shear_, -y_shear_, p);
  mouse_x = p[0] + x_;
  mouse_y = p[1] + y_;

  if (selected) {
    for (BorderRect &rect : border_rects_) {
      if (mouse_x > x_ + rect.x && mouse_x < x_ + rect.x + rect.width &&
        mouse_y > y_ + rect.y && mouse_y < y_ + rect.y + rect.height)
      {
        for (BorderRect &rect : border_rects_) {
          rect.selected = false;
        }
        rect.selected = true;
        return true;
      }
      else {
        rect.selected = false;
      }
    }
    for (BorderRect &rect : shear_rects) {
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

  for (BorderRect &rect : border_rects_) {
    rect.selected = false;
  }
  for (BorderRect &rect : shear_rects) {
    rect.selected = false;
  }

  if (mouse_x > x_ - x_radius_ && mouse_x < x_ + x_radius_ &&
    mouse_y > y_ - y_radius_ && mouse_y < y_ + y_radius_)
  {
    return true;
  }
  else {
    return false;
  }
}

void CRectangle::ReceiveMouseClick(SDL_MouseButtonEvent event) {
  click_type_ = event.button;
}

void CRectangle::ReceiveMouseMotion(float mouse_x_offset, float mouse_y_offset)
{
  if (click_type_ == SDL_BUTTON_LEFT) {
    for (BorderRect rect : border_rects_) {
      if (rect.selected)
      {
        Resize(mouse_x_offset, mouse_y_offset, rect.position);
        return;
      }
    }

    for (BorderRect rect : shear_rects) {
      if (rect.selected)
      {
        Shear(mouse_x_offset, mouse_y_offset, rect.position);
        return;
      }
    }

    //If none of the border rects are selected, it means that the this CCircle itself is selected, therefore it moves.
    Move(mouse_x_offset, mouse_y_offset);
  }
  else {
    Rotate(mouse_x_offset, mouse_y_offset);
  }
}

void CRectangle::Move(float mouse_x_offset, float mouse_y_offset)
{
  int screenWidth;
  int screenHeight;
  int vPort[4];

  glGetIntegerv(GL_VIEWPORT, vPort);
  screenWidth = vPort[2];
  screenHeight = vPort[3];

  x_ += mouse_x_offset / (screenWidth / 2);
  y_ += mouse_y_offset / (screenHeight / 2);
}

void CRectangle::Resize(float mouse_x_offset, float mouse_y_offset, BorderRectPosition position)
{
  int screenWidth;
  int screenHeight;
  int vPort[4];
  float p[2];

  glGetIntegerv(GL_VIEWPORT, vPort);
  screenWidth = vPort[2];
  screenHeight = vPort[3];

  mouse_x_offset = mouse_x_offset / (screenWidth / 2);
  mouse_y_offset = mouse_y_offset / (screenHeight / 2);

  rotatePoint(mouse_x_offset, mouse_y_offset, -angle_, p);
  mouse_x_offset = p[0];
  mouse_y_offset = p[1];

  switch (position) {

  case BorderRectPosition::LEFT:
    if (width_ - mouse_x_offset > 0) {
      x_ += mouse_x_offset / 2;
      width_ -= mouse_x_offset;
    }
    break;
  case BorderRectPosition::RIGHT:
    if (width_ + mouse_x_offset > 0) {
      x_ += mouse_x_offset / 2;
      width_ += mouse_x_offset;
    }
    break;
  case BorderRectPosition::TOP:
    if (height_ + mouse_y_offset > 0) {
      y_ += mouse_y_offset / 2;
      height_ += mouse_y_offset;
    }
    break;
  case BorderRectPosition::BOTTOM:
    if (height_ - mouse_y_offset > 0) {
      y_ += mouse_y_offset / 2;
      height_ -= mouse_y_offset;
    }
    break;
  case BorderRectPosition::BOTTOM_LEFT:
    if (width_ - mouse_x_offset > 0 &&
      height_ - mouse_y_offset > 0)
    {
      if (abs(mouse_x_offset) >= abs(mouse_y_offset)) {
        x_ += mouse_x_offset / 2;
        width_ -= mouse_x_offset;
        y_ += mouse_x_offset / 2;
        height_ -= mouse_x_offset;
      }
      else {
        x_ += mouse_y_offset / 2;
        width_ -= mouse_y_offset;
        y_ += mouse_y_offset / 2;
        height_ -= mouse_y_offset;
      }
    }
    break;

  case BorderRectPosition::BOTTOM_RIGHT:
    if (width_ + mouse_x_offset > 0 &&
      height_ - mouse_y_offset > 0)
    {
      if (abs(mouse_x_offset) >= abs(mouse_y_offset)) {
        x_ += mouse_x_offset / 2;
        width_ += mouse_x_offset;
        y_ -= mouse_x_offset / 2;
        height_ += mouse_x_offset;
      }
      else {
        x_ -= mouse_y_offset / 2;
        width_ -= mouse_y_offset;
        y_ += mouse_y_offset / 2;
        height_ -= mouse_y_offset;
      }
    }
    break;

  case BorderRectPosition::TOP_LEFT:
    if (width_ - mouse_x_offset > 0 &&
      height_ - mouse_y_offset > 0)
    {
      if (abs(mouse_x_offset) >= abs(mouse_y_offset)) {
        x_ += mouse_x_offset / 2;
        width_ -= mouse_x_offset;
        y_ -= mouse_x_offset / 2;
        height_ -= mouse_x_offset;
      }
      else {
        x_ -= mouse_y_offset / 2;
        width_ += mouse_y_offset;
        y_ += mouse_y_offset / 2;
        height_ += mouse_y_offset;
      }
    }
    break;

  case BorderRectPosition::TOP_RIGHT:
    if (width_ + mouse_x_offset > 0 &&
      height_ + mouse_y_offset > 0)
    {
      if (abs(mouse_x_offset) >= abs(mouse_y_offset)) {
        x_ += mouse_x_offset / 2;
        width_ += mouse_x_offset;
        y_ += mouse_x_offset / 2;
        height_ += mouse_x_offset;
      }
      else {
        x_ += mouse_y_offset / 2;
        width_ += mouse_y_offset;
        y_ += mouse_y_offset / 2;
        height_ += mouse_y_offset;
      }
    }
    break;
  }

  x_radius_ = width_ / 2;
  y_radius_ = height_ / 2;

  set_border_rects_();
  set_shear_rects();
}

void CRectangle::Rotate(float mouse_x_offset, float mouse_y_offset)
{
  int screenWidth;
  int screenHeight;
  int vPort[4];

  glGetIntegerv(GL_VIEWPORT, vPort);
  screenWidth = vPort[2];
  screenHeight = vPort[3];

  mouse_x_offset = mouse_x_offset / (screenWidth / 2);
  mouse_y_offset = mouse_y_offset / (screenHeight / 2);

  angle_ += mouse_x_offset * 360;
}

void CRectangle::Shear(float mouse_x_offset, float mouse_y_offset, BorderRectPosition position)
{
  int screenWidth;
  int screenHeight;
  int vPort[4];
  float p[2];

  glGetIntegerv(GL_VIEWPORT, vPort);
  screenWidth = vPort[2];
  screenHeight = vPort[3];

  mouse_x_offset = mouse_x_offset / (screenWidth / 2);
  mouse_y_offset = mouse_y_offset / (screenHeight / 2);

  rotatePoint(mouse_x_offset, mouse_y_offset, -angle_, p);
  mouse_x_offset = p[0];
  mouse_y_offset = p[1];

  switch (position) {

  case BorderRectPosition::RIGHT:
    y_shear_ += mouse_y_offset;
    break;
  case BorderRectPosition::TOP:
    x_shear_ += mouse_x_offset;
    break;
  }
}