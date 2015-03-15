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
  void HandleClick(int mouse_x, int mouse_y);
  void CreateQuad();

	const int SCREEN_WIDTH;
	const int SCREEN_HEIGHT;
	SDL_Window* pSDLWindow_;
	InputHandler inputHandler_;
	bool quit;
	int previousFrameTime;
	int currentFrameTime;
  std::list<IShape *> shapes_;
  IButton* button_;
};