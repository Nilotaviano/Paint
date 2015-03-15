#pragma once
#include "IShape.h"
class CCircle :
  public IShape
{
public:
  CCircle(float x, float y, float height, float width, unsigned char r = 128, unsigned char g = 128, unsigned char b = 128, float rotation = 0);
  ~CCircle();
  void Update();
  void Draw();
  void Move(float x_offset, float y_offset);
  void Resize(float x_offset, float y_offset, BorderRectPosition position);
  void Rotate(float x_offset, float y_offset);
  bool IsMouseOver(float mouse_x, float mouse_y);
  void ReceiveMouseMotion(float mouse_x_offset, float mouse_y_offset);

private:
  float x_;
  float y_;
  float width_;
  float height_;

  unsigned char r_;
  unsigned char g_;
  unsigned char b_;

  float rotation_;

  //These rectangles will be drawn on this circle's borders when selected = true
  BorderRect border_rects_[8];
  void set_border_rects_();
};