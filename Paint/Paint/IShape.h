#pragma once

class IShape
{
public:
  virtual ~IShape() {}
  virtual void Draw() = 0;
  virtual void Update() = 0;
};