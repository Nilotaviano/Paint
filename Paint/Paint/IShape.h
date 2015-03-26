#pragma once

#include <SDL.h>
#include <stdio.h>
#include <math.h>

class IShape
{
public:
  IShape();

  void rotatePoint(float point[2], float angle) {
    float rad = (-angle) * M_PI / 180;
    float s = sin(rad);
    float c = cos(rad);
    float new_p_x = point[0] * c - point[1] * s;
    float new_p_y = point[0] * s + point[1] * c;

    point[0] = new_p_x;
    point[1] = new_p_y;
  }

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