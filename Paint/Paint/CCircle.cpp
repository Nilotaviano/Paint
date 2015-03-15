#include "CCircle.h"

#include <SDL_opengl.h>
#include <GL\GLU.h>
#include <math.h>
#include <stdio.h>

CCircle::CCircle(float x, float y, float height, float width, unsigned char r, unsigned char g, unsigned char b, float angle)
: x_(x), y_(y),
width_(width), height_(height),
r_(r), g_(g), b_(b),
angle_(45),
IShape()
{
  set_border_rects_();
}

void CCircle::set_border_rects_() {
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

CCircle::~CCircle()
{

}

void CCircle::Draw()
{
  int triangle_amount = 50;
  float twice_pi_over_triangle_amount = (2.0f * M_PI) / triangle_amount;
  float x_radius = width_ / 2;
  float y_radius = height_ / 2;

  glPushMatrix();
  
  glTranslatef(x_, y_, 0);


  //Center on origin before angle
  glTranslatef((x_radius), (y_radius), 0);
  glRotatef(angle_, 0, 0, 1.0f);
  glTranslatef(-(x_radius), -(y_radius), 0);


  glColor3ub(r_, g_, b_);

  //Filled circle
  glBegin(GL_TRIANGLE_FAN);
  glVertex2f(x_radius, y_radius); //Center of circle
  for (int i = 0; i <= triangle_amount; i++) {
    glVertex2f(
      x_radius + (x_radius * cos(i * twice_pi_over_triangle_amount)),
      y_radius + (y_radius * sin(i * twice_pi_over_triangle_amount))
      );
  }
  glEnd();

  glLineWidth(2.5);
  glColor3f(0.0f, 0.0f, 0.0f);

  //Outline
  glBegin(GL_LINE_LOOP);
  for (int i = 0; i <= triangle_amount; i++) {
    glVertex2f(
      x_radius + (x_radius * cos(i * twice_pi_over_triangle_amount)),
      y_radius + (y_radius * sin(i * twice_pi_over_triangle_amount))
      );
  }
  glEnd();

  //Will Draw 4 small gray squares on each corner of this circle (Like it's a rectangle)
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

void CCircle::Update()
{

}

bool CCircle::IsMouseOver(float mouse_x, float mouse_y)
{
  float x_radius = width_ / 2;
  float y_radius = height_ / 2;
  float click_focus;

  //Convert mouse coordinates to "my world" coordinates
  mouse_x = (mouse_x - 320) / 320;
  mouse_y = -(mouse_y - 480);
  mouse_y = (mouse_y - 240) / 240;

  /*Rotate point*/
  if (angle_ > 0) {
    float rad = angle_ * M_PI / 180;
    float s = sin(rad);
    float c = cos(rad);
    //Translate back to origin
    mouse_x -= (x_ + x_radius);
    mouse_y -= (y_ + y_radius);
    //Rotate around origin
    float new_mouse_x = mouse_x * c - mouse_y * s;
    float new_mouse_y = mouse_x * s + mouse_y * c;
    //Translate back
    mouse_x = new_mouse_x + (x_ + x_radius);
    mouse_y = new_mouse_y + (y_ + y_radius);
  }


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

  click_focus = (pow(mouse_x - (x_ + x_radius), 2) / pow(x_radius, 2)) + (pow(mouse_y - (y_ + y_radius), 2) / pow(y_radius, 2));

  if (click_focus <= 1) {
    return true;
  }
  else {
    return false;
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

void CCircle::ReceiveMouseMotion(float mouse_x_offset, float mouse_y_offset)
{
  for (BorderRect rect : border_rects_) {
    if (rect.selected)
    {
      Resize(mouse_x_offset, mouse_y_offset, rect.position);
      return;
    }
  }

  //If none of the border rects are selected, it means that the this CCircle itself is selected, therefore it moves.
  Move(mouse_x_offset, mouse_y_offset);
}

void CCircle::Move(float mouse_x_offset, float mouse_y_offset)
{
  x_ += mouse_x_offset / 320;
  y_ += mouse_y_offset / 240;
}

void CCircle::Resize(float mouse_x_offset, float mouse_y_offset, BorderRectPosition position)
{
  mouse_x_offset = mouse_x_offset / 320;
  mouse_y_offset = mouse_y_offset / 240;

  switch (position) {

  case BorderRectPosition::LEFT:
    if (width_ - mouse_x_offset > 0) {
      x_ += mouse_x_offset;
      width_ -= mouse_x_offset;
    }
    break;
  case BorderRectPosition::RIGHT:
    if (width_ + mouse_x_offset > 0) {
      width_ += mouse_x_offset;
  }
    break;
  case BorderRectPosition::TOP:
    if (height_ + mouse_y_offset > 0) {
      height_ += mouse_y_offset;
    }
    break;
  case BorderRectPosition::BOTTOM:
    if (height_ - mouse_y_offset > 0) {
      y_ += mouse_y_offset;
      height_ -= mouse_y_offset;
    }
    break;
  case BorderRectPosition::BOTTOM_LEFT:
    if (width_ - mouse_x_offset > 0 &&
        height_ - mouse_y_offset > 0)
    {
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
    }
    break;

  case BorderRectPosition::BOTTOM_RIGHT:
    if (width_ + mouse_x_offset > 0 &&
        height_ - mouse_y_offset > 0)
    {
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
    }
    break;

  case BorderRectPosition::TOP_LEFT:
    if (width_ - mouse_x_offset > 0 &&
        height_ + mouse_y_offset > 0)
    {
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
    }
    break;

  case BorderRectPosition::TOP_RIGHT:
    if (width_ + mouse_x_offset > 0 &&
      height_ + mouse_y_offset > 0)
    {
      if (abs(mouse_x_offset) >= abs(mouse_y_offset)) {
        width_ += mouse_x_offset;
        height_ += mouse_x_offset;
      }
      else {
        width_ += mouse_y_offset;
        height_ += mouse_y_offset;
      }
    }
    break;
  }

  set_border_rects_();
}

void CCircle::Rotate(float mouse_x_offset, float mouse_y_offset)
{
  mouse_x_offset = mouse_x_offset / 320;
  mouse_y_offset = mouse_y_offset / 240;

  angle_ += mouse_x_offset * 360;
}