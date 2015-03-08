#pragma once
#include "IShape.h"

#include <SDL.h>

class CRectangle :
  public IShape
{
public:
  CRectangle(float x, float y, float height, float width, unsigned char r = 128, unsigned char g = 128, unsigned char b = 128, int rotation = 0);
  ~CRectangle();
  void Update();
  void Draw();
  void Move(float x_offset, float y_offset);
  void Resize(float x_offset, float y_offset);
  void Rotate(float x_offset, float y_offset);
  bool IsMouseOver(float mouse_x, float mouse_y);

private:
  float x_;
  float y_;
  float width_;
  float height_;

  unsigned char r_;
  unsigned char g_;
  unsigned char b_;

  int rotation_;

  struct BorderRect
  {
    float x;
    float y;
    float width;
    float height;
  };

  //These rectangles will be drawn on this rectangle's borders when selected = true
  BorderRect border_rects_[8];
};