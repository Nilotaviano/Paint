#include "ImageButton.h"
#include <SDL_opengl.h>

ImageButton::ImageButton(float x, float y, float width, float height, int r, int g, int b, std::function<void()> callback_function, int shape)
: callback_function_(callback_function),
shape_(shape)
{
  selected = false;
  x_ = x;
  y_ = y;
  width_ = width;
  height_ = height;

  color_.r = r;
  color_.g = g;
  color_.b = b;

}


ImageButton::~ImageButton()
{
}

void ImageButton::Draw() {
  glColor3ub(color_.r, color_.g, color_.b);
  glRectf(
    x_,
    y_,
    x_ + width_,
    y_ + height_);

  if (shape_ == GL_QUADS) {
    glColor3ub(255, 0, 0);
    glRectf(
      x_ + 0.025f,
      y_ + 0.025f,
      x_ + width_ - 0.025f,
      y_ + height_ - 0.025f);
  }
  else if (shape_ == GL_TRIANGLE_FAN) {
    float twice_pi_over_triangle_amount = (2.0f * M_PI) / 25;

    glPushMatrix();

    glTranslatef(x_ + width_ / 2, y_ + height_ / 2, 0);
    glColor3ub(0, 255, 0);

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0, 0); //Center of circle
    for (int i = 0; i <= 25; i++) {
      glVertex2f(
        (0.025f * cos(i * twice_pi_over_triangle_amount)),
        (0.025f * sin(i * twice_pi_over_triangle_amount))
        );
    }
    glEnd();

    glPopMatrix();
  }
}

bool ImageButton::IsMouseOver(float mouse_x, float mouse_y)
{
  //Convert mouse coordinates to "my world" coordinates
  int screenWidth;
  int screenHeight;
  int vPort[4];

  glGetIntegerv(GL_VIEWPORT, vPort);
  screenWidth = vPort[2];
  screenHeight = vPort[3];

  mouse_x = (mouse_x - (screenWidth / 2)) / (screenWidth / 2);
  mouse_y = -(mouse_y - screenHeight);
  mouse_y = (mouse_y - (screenHeight / 2)) / (screenHeight / 2);

  if (mouse_x > x_ && mouse_x < x_ + width_ &&
    mouse_y > y_ && mouse_y < y_ + height_)
  {
    return true;
  }
  else {
    return false;
  }
}

void ImageButton::HandleClick() {
  callback_function_();
}