#pragma once

#include <SDL.h>
#include <stdio.h>
#include <math.h>
#include <SDL_opengl.h>

class IShape
{
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

public:
  IShape();

  void rotatePoint(float x, float y, float angle, float point[2]) {
    float rad = angle * M_PI / 180;
    float s = sin(rad);
    float c = cos(rad);

    point[0] = x * c - y * s;
    point[1] = x * s + y * c;
  }

  void shearPoint(float x, float y, float x_shear, float y_shear, float p[2]) {
    p[0] = x + y * x_shear;
    p[1] = y + x * y_shear;
  }

  void shearMatrix(float x_shear, float y_shear) {
    float m[] = {
      1, y_shear, 0, 0,
      x_shear, 1, 0, 0,
      0, 0, 1, 0,
      0, 0, 0, 1 };
    glMultMatrixf(m);
  }

  virtual ~IShape() {}

  virtual void Draw() {}

  virtual void Update() {}

  virtual void Move(float x_offset, float y_offset) {}

  virtual void Resize(float x_offset, float y_offset) {}

  virtual void Shear(float mouse_x_offset, float mouse_y_offset, BorderRectPosition position) {}

  virtual bool IsMouseOver(float mouse_x, float mouse_y) { return false; }

  virtual void ReceiveMouseClick(SDL_MouseButtonEvent event) { printf("IReceiveMouseClick\n"); }

  virtual void ReceiveMouseMotion(float mouse_x_offset, float mouse_y_offset) { printf("IReceiveMouseMotion\n"); }

  bool selected;
};