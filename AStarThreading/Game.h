#pragma once
#include "Renderer.h"
#include "InputManager.h"
#include "EventListener.h"
#include <vector>

/** The game objct whic manages the game loop*/
class Game : public EventListener
{
	InputManager *inputManager;
	Renderer renderer;
	//time of last update;
	unsigned int lastTime;
	bool m_quit;

public:
	Game();
	~Game();

	bool init();
	void destroy();

	void update();
	void render();
	void loop();

	void onEvent(EventListener::Event evt);
};