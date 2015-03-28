#pragma once
#include "IShape.h"

class CRectangle :
  public IShape
{
public:
  CRectangle(float x, float y, float height, float width, unsigned char r = 128, unsigned char g = 128, unsigned char b = 128, float angle = 0);
  ~CRectangle();
  void Draw();
  void Move(float x_offset, float y_offset);
  void Resize(float x_offset, float y_offset, BorderRectPosition position);
  void Rotate(float x_offset, float y_offset);
  void Shear(float mouse_x_offset, float mouse_y_offset, BorderRectPosition position);
  bool IsMouseOver(float mouse_x, float mouse_y);
  void ReceiveMouseClick(SDL_MouseButtonEvent event);
  void ReceiveMouseMotion(float mouse_x_offset, float mouse_y_offset);

private:
  float x_;
  float y_;
  float width_;
  float height_;
  float x_radius_;
  float y_radius_;
  float x_shear_;
  float y_shear_;

  unsigned char r_;
  unsigned char g_;
  unsigned char b_;

  float angle_;
  int click_type_;  //SDL_BUTTON_LEFT SDL_BUTTON_RIGHT

  //These rectangles will be drawn on this rectangle's borders when selected = true
  BorderRect border_rects_[8];
  BorderRect shear_rects[2];
  void set_border_rects_();
  void set_shear_rects();
};