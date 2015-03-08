#pragma once
#include "IShape.h"
class Rectangle :
  public IShape
{
public:
  Rectangle(float x, float y, float height, float width, int rotation = 0);
  ~Rectangle();
  void Update();
  void Draw();
  void Move(float x_offset, float y_offset);

  bool selected;

private:
  float x_;
  float y_;
  float width_;
  float height_;
  int rotation_;
};