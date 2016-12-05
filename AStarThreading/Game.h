#pragma once
#include "Renderer.h"
#include "InputManager.h"
#include "EventListener.h"
#include "Graph.h"
#include "Tile.h"
#include "Player.h"
#include "Enemy.h"
#include <vector>
#include <time.h>

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
	Player * m_player;
	Size m_screenSize;
	int m_noOfEnemies;
	std::vector<Enemy *> m_enemies;
	int m_vpWidth;
	int m_totalWalls;
	int m_touchingWalls;

public:
	Game();
	~Game();

	bool init();
	void reinitialise();
	bool initGraph();
	bool initEnemies();
	void destroy();

	void update();
	void render();
	void loop();

	void onEvent(EventListener::Event evt);
};