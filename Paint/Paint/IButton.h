#pragma once
class IButton
{
public:
  IButton();
  virtual ~IButton();
  virtual void Draw() {}
  virtual bool IsMouseOver(float mouse_x, float mouse_y) { return false; }
  virtual void HandleClick() {}
};

