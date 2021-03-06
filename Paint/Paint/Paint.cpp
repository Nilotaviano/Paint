﻿#include "Paint.h"

#include <stdio.h>
#include <SDL_opengl.h>
#include <GL\GLU.h>

#include "CRectangle.h"
#include "CCircle.h"
#include "ImageButton.h"

Paint::Paint()
  : pSDLWindow_(nullptr), quit(false),
  SCREEN_WIDTH(640), SCREEN_HEIGHT(480),
  inputHandler_(&quit, std::bind(&Paint::Resize, this), std::bind(&Paint::HandleClick, this, std::placeholders::_1)),
  p_quad_button_(new ImageButton(-.95f, .05f, .1f, .1f, std::bind(&Paint::CreateQuad, this, std::placeholders::_1), 4, true)),
  p_circle_button_(new ImageButton(-.95f, -.15f, .1f, .1f, std::bind(&Paint::CreateCircle, this, std::placeholders::_1), 25, true))
{
}

Paint::~Paint()
{
}

//SDL initialization tutorial source: http://lazyfoo.net/SDL_tutorials/
bool Paint::Init()
{
  //Initialization flag
  bool success = true;

  //Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
    success = false;
  }
  else
  {
    //OpenGL 2.1
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    //Create window
    pSDLWindow_ = SDL_CreateWindow("Projeto CG - Paint 2.0 - Nilo Otaviano", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (pSDLWindow_ == nullptr)
    {
      printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
      success = false;
    }
    else
    {
      //Create context
      SDL_GLContext glContext = SDL_GL_CreateContext(pSDLWindow_);
      if (glContext == nullptr)
      {
        printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
        success = false;
      }
      else
      {
        //Use Vsync
        if (SDL_GL_SetSwapInterval(1) < 0)
        {
          printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
        }

        //Initialize OpenGL
        if (!InitGL())
        {
          printf("Unable to initialize OpenGL!\n");
          success = false;
        }
      }
    }
  }

  return success;
}

bool Paint::InitGL()
{
  bool success = true;
  GLenum error = GL_NO_ERROR;
  int x, y;

  SDL_GetWindowSize(pSDLWindow_, &x, &y);
  glViewport(0, 0, x, y);

  //Initialize Projection Matrix
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  //Check for error
  error = glGetError();
  if (error != GL_NO_ERROR)
  {
    printf("Error initializing OpenGL! %s\n", gluErrorString(error));
    success = false;
  }

  //Initialize Modelview Matrix
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  //Check for error
  error = glGetError();
  if (error != GL_NO_ERROR)
  {
    printf("Error initializing OpenGL! %s\n", gluErrorString(error));
    success = false;
  }

  //Initialize clear color
  glClearColor(1.0f, 1.0f, 1.0f, .0f);

  //Check for error
  error = glGetError();
  if (error != GL_NO_ERROR)
  {
    printf("Error initializing OpenGL! %s\n", gluErrorString(error));
    success = false;
  }

  return success;
}

void Paint::Close()
{
  //Destroy window	
  SDL_DestroyWindow(pSDLWindow_);
  pSDLWindow_ = NULL;

  //Quit SDL subsystems
  SDL_Quit();
}

void Paint::Update()
{
}

void Paint::Draw()
{
  glClear(GL_COLOR_BUFFER_BIT);

  glColor3f(0, 0, 0);
  glLineWidth(.5);
  glBegin(GL_LINES);
  for (float i = -1; i <= 1; i += 0.1f) {
    glVertex2f(i, -1);
    glVertex2f(i, 1);
    glVertex2f(-1, i);
    glVertex2f(1, i);
  }
  glEnd();

  for (std::list<IShape *>::reverse_iterator iterator = shapes_.rbegin(); iterator != shapes_.rend(); iterator++) {
    (*iterator)->Draw();
  }
  p_quad_button_->Draw();
  p_circle_button_->Draw();
}

void Paint::Resize()
{
  int w, h;
  SDL_GetWindowSize(pSDLWindow_, &w, &h);

  glViewport(0, 0, w, h);

  glMatrixMode(GL_PROJECTION);
  glOrtho(0, w, 0, h, -1, 1);

  glLoadIdentity();

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void Paint::HandleClick(SDL_MouseButtonEvent event)
{
  if (p_quad_button_->IsMouseOver(event.x, event.y)) {
    p_circle_button_->IsMouseOver(event.x, event.y);
    p_quad_button_->HandleClick();
  }
  else if (p_circle_button_->IsMouseOver(event.x, event.y)) {
    p_circle_button_->HandleClick();
  }
  else {
    for (IShape* shape : shapes_) {
      if (shape->IsMouseOver(event.x, event.y)) {
        shape->selected = true;

        //Gotta remove the shape from current position and push_front, so it's checked for mouse over first and drawn last.
        shapes_.remove(shape);
        shapes_.push_front(shape);
        inputHandler_.set_p_shape_(shape);
        shape->ReceiveMouseClick(event);
        return;
      }
    }
  }
  //Must remove previous shape on inputHandler_ if no shape was selected.
  inputHandler_.set_p_shape_(nullptr);
}

void Paint::CreateQuad(SDL_Color color)
{
  shapes_.push_front(new CRectangle(-.0f, -.0f, .2, .2, color.r, color.g, color.b));
}

void Paint::CreateCircle(SDL_Color color) {
  shapes_.push_front(new CCircle(-.0f, -.0f, .2, .2, color.r, color.g, color.b));
}

void Paint::Run() {
  if (!Init())
  {
    printf("Failed to initialize!\n");
  }
  else
  {
    SDL_Event event;
    current_frame_time_ = SDL_GetTicks();
    previous_frame_time_ = current_frame_time_;

    while (!quit)
    {
      //Handle events on queue
      while (SDL_PollEvent(&event) != 0)
      {
        inputHandler_.handleInput(event);
      }

      if (SDL_GetTicks() - previous_frame_time_ > 0) {
        current_frame_time_ = SDL_GetTicks();
        Update();
        Draw();
        previous_frame_time_ = current_frame_time_;
      }

      //Update screen
      SDL_GL_SwapWindow(pSDLWindow_);
    }
  }

  //Free resources and Close SDL
  Close();
}