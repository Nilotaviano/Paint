#pragma once

class IShape
{
public:
  IShape();

  virtual ~IShape() {}

  virtual void Draw() {}

  virtual void Update() {}

  virtual void Move(float x_offset, float y_offset) {}

  virtual void Resize(float x_offset, float y_offset) {}

  virtual bool IsMouseOver(float mouse_x, float mouse_y) { return false; }

  bool selected;
};