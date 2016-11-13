#pragma once
#include "Renderer.h"
#include "InputManager.h"
#include "EventListener.h"
#include "Graph.h"
#include "Tile.h"
#include <vector>

/** The game objct whic manages the game loop*/
class Game : public EventListener
{
private:
	InputManager *inputManager;
	Renderer * m_renderer;
	//time of last update;
	unsigned int lastTime;
	bool m_quit;

	Graph<Tile *> * m_graph;
	int m_vpWidth;

public:
	Game();
	~Game();

	bool init();
	bool initGraph();
	void destroy();

	void update();
	void render();
	void loop();

	void onEvent(EventListener::Event evt);
};