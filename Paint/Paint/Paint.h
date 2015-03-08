#pragma once

#include <SDL.h>

#include "InputHandler.h"

class Paint
{
public:
	Paint();
	~Paint();
	void update();
	void draw();
	void run();
private:
	bool init();
	bool initGL();
  void resize();
	void close();
	const int SCREEN_WIDTH;
	const int SCREEN_HEIGHT;
	SDL_Window* pSDLWindow_;
	InputHandler inputHandler_;
	bool quit;
	int previousFrameTime;
	int currentFrameTime;
};