#pragma once

#include "SDL.h"

#include "BasicTypes.h"

//Responsible for all drawing operations
//abstracts away specfic SDL specific drawing functions
class Renderer {

	// size of window in pixels
	Size windowSize;
	//position of window in world coordinates
	//change these if you want to zoom or pan
	Vector2 viewportBottomLeft;
	Size viewportSize;
	SDL_Window *window;
	SDL_Renderer *sdl_renderer;

public:
	Renderer();
	~Renderer();
	bool init(const Size&, const char*);
	void destroy();
	void drawRect(const Rect&, const Colour&) const;
	void drawFillRect(const Rect&, const Colour&) const;
	void drawWorldRect(const Rect&, const Colour&) const;
	void drawWorldFillRect(const Rect&, const Colour&) const;
	void present();
	void clear(const Colour&) const;
	Vector2 worldToScreen(const Vector2&) const;
	Rect worldToScreen(const Rect&) const;
	void setViewPort(const Rect&);
	Size getWindowSize() const;
};