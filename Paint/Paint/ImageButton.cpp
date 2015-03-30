#include "ImageButton.h"
#include <SDL_opengl.h>

ImageButton::ImageButton(float x, float y, float width, float height, std::function<void(SDL_Color)> callback_function, int sides, bool is_parent)
: callback_function_(callback_function),
num_sides_(sides), is_parent_(is_parent)
{
  is_selected_ = false;
  x_ = x;
  y_ = y;
  width_ = width;
  height_ = height;

  if (is_parent_) {
    buttons_.push_front(ImageButton(x + 0.11f, y_ + 0.11f, width, height, 255, 0, 0, callback_function, sides, false));
    buttons_.push_front(ImageButton(x + 0.11f, y_, width, height, 0, 255, 0, callback_function, sides, false));
    buttons_.push_front(ImageButton(x + 0.11f, y_ - 0.11f, width, height, 0, 0, 255, callback_function, sides, false));
  }
}

ImageButton::ImageButton(float x, float y, float width, float height, int r, int g, int b, std::function<void(SDL_Color)> callback_function, int sides, bool is_parent)
: callback_function_(callback_function),
num_sides_(sides), is_parent_(is_parent)
{
  is_selected_ = false;
  x_ = x;
  y_ = y;
  width_ = width;
  height_ = height;

  color_.r = r;
  color_.g = g;
  color_.b = b;

  if (is_parent_) {
    buttons_.push_front(ImageButton(x + 0.06f, y_ + 0.06f, width, height, 255, 0, 0, callback_function, sides, false));
    buttons_.push_front(ImageButton(x + 0.06f, y_, width, height, 0, 255, 0, callback_function, sides, false));
    buttons_.push_front(ImageButton(x + 0.06f, y_ - 0.06f, width, height, 0, 0, 255, callback_function, sides, false));
  }
}


ImageButton::~ImageButton()
{
}

void ImageButton::Draw() {
  float twice_pi_over_triangle_amount = (2.0f * M_PI) / num_sides_;

  if (is_parent_) {
    glColor3ub(0, 0, 0);
    glRectf(
      x_,
      y_,
      x_ + width_,
      y_ + height_);

    glPushMatrix();

    glTranslatef(x_ + width_ / 2, y_ + height_ / 2, 0);

    glRotatef(45, 0, 0, 1.0f);

    glColor3ub(255, 255, 255);

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0, 0); //Center of circle
    for (int i = 0; i <= num_sides_; i++) {
      glVertex2f(
        (0.035f * cos(i * twice_pi_over_triangle_amount)),
        (0.035f * sin(i * twice_pi_over_triangle_amount))
        );
    }
    glEnd();

    glPopMatrix();

    if (is_selected_) {
      for (ImageButton bt : buttons_) {
        bt.Draw();
      }
    }
  }
  else {
    glColor3ub(0, 0, 0);
    glRectf(
      x_,
      y_,
      x_ + width_,
      y_ + height_);

    glPushMatrix();

    glTranslatef(x_ + width_ / 2, y_ + height_ / 2, 0);

    glRotatef(45, 0, 0, 1.0f);

    glColor3ub(color_.r, color_.g, color_.b);

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0, 0); //Center of circle
    for (int i = 0; i <= num_sides_; i++) {
      glVertex2f(
        (0.035f * cos(i * twice_pi_over_triangle_amount)),
        (0.035f * sin(i * twice_pi_over_triangle_amount))
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

  if (is_selected_) {
    bool child_selected = false;
    for (ImageButton &bt : buttons_) {
      if (bt.IsMouseOver(mouse_x, mouse_y)) {
        child_selected = true;
      }
    }
    if (child_selected) {
      return true;
    }
  }

  glGetIntegerv(GL_VIEWPORT, vPort);
  screenWidth = vPort[2];
  screenHeight = vPort[3];

  mouse_x = (mouse_x - (screenWidth / 2)) / (screenWidth / 2);
  mouse_y = -(mouse_y - screenHeight);
  mouse_y = (mouse_y - (screenHeight / 2)) / (screenHeight / 2);

  if (mouse_x > x_ && mouse_x < x_ + width_ &&
    mouse_y > y_ && mouse_y < y_ + height_)
  {
    is_selected_ = true;
    return true;
  }
  else {
    is_selected_ = false;
    return false;
  }
}

void ImageButton::HandleClick() {
  if (is_parent_) {
    for (ImageButton &bt : buttons_) {
      bt.HandleClick();
    }
  }
  else {
    if (is_selected_) {
      callback_function_(color_);
    }
  }
}