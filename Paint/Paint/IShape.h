#pragma once

class IShape
{
public:
  IShape();

  virtual ~IShape() {}

  virtual void Draw() {}

  virtual void Update() {}

  virtual void Move(float x_offset, float y_offset) {}

  bool selected;
};