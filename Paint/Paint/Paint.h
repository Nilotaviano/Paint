#pragma once

#include <SDL.h>
#include <list>

#include "InputHandler.h"
#include "IShape.h"
#include "IButton.h"

class Paint
{
public:
	Paint();
	~Paint();
	void Update();
	void Draw();
	void Run();
private:
	bool Init();
	bool InitGL();
  void Resize();
	void Close();
  void HandleClick(SDL_MouseButtonEvent event);
  void CreateQuad(SDL_Color color);
  void CreateCircle(SDL_Color color);

	const int SCREEN_WIDTH;
	const int SCREEN_HEIGHT;
	SDL_Window* pSDLWindow_;
	InputHandler inputHandler_;
	bool quit;
	int previous_frame_time_;
	int current_frame_time_;
  std::list<IShape *> shapes_;
  IButton* p_quad_button_;
  IButton* p_circle_button_;
};