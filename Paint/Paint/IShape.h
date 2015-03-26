#pragma once

#include <SDL.h>
#include <stdio.h>

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

  virtual void ReceiveMouseClick(SDL_MouseButtonEvent event) { printf("IReceiveMouseClick\n"); }

  virtual void ReceiveMouseMotion(float mouse_x_offset, float mouse_y_offset) { printf("IReceiveMouseMotion\n"); }

  bool selected;

protected:
  enum class BorderRectPosition {
    LEFT,
    RIGHT,
    TOP,
    BOTTOM,
    BOTTOM_LEFT,
    BOTTOM_RIGHT,
    TOP_LEFT,
    TOP_RIGHT
  };

  struct BorderRect
  {
    float x;
    float y;
    float width;
    float height;
    BorderRectPosition position;
    bool selected;
  };
};