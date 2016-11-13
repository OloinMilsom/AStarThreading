#include "Game.h"
#include <iostream>

const int SCREEN_FPS = 100;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

Game::Game()
{
	inputManager = new InputManager();
	m_graph = new Graph<Tile *>(500 * 500, &Tile::manhattanDistance);
	m_quit = false;
	m_vpWidth = 30;
}

Game::~Game()
{

}

bool Game::init()
{
	Size winSize(1000, 1000);

	//creates our renderer, which looks after drawing and the window
	m_renderer = new Renderer();
	m_renderer->init(winSize, "AStarThreading");

	//set up the viewport
	//we want the vp centred on origin and 20 units wide
	float aspectRatio = winSize.w / winSize.h;
	Size vpSize(m_vpWidth, m_vpWidth / aspectRatio);
	Vector2 vpBottomLeft(-vpSize.w / 2, -vpSize.h / 2);

	Rect vpRect(vpBottomLeft, vpSize);
	m_renderer->setViewPort(vpRect);

	lastTime = SDL_GetTicks();

	initGraph();

	inputManager->AddListener(EventListener::Event::QUIT, this);

	return true;
}

bool Game::initGraph()
{
	int size = m_vpWidth;
	bool success = true;
	Colour light(159, 129, 112);
	Colour dark(245, 245, 220);
	for (int i = 0; i < size * size; i++) {
		Tile * tile = new Tile(Rect(i % size, i / size, 1, 1), i % 2 == 0 ? ((i/ size) % 2 == 0 ? light : dark) : (i / size) % 2 == 0 ? dark : light);
		success &= m_graph->addNode(tile, i);
	}
	for (int i = 0; i < size * size; i++) {
		if (i / size != 0) {
			m_graph->addArc(i, i - size);
		}
		if (i / size != size - 1) {
			m_graph->addArc(i, i + size);
		}
		if (i % size != 0) {
			m_graph->addArc(i, i - 1);
		}
		if (i % size != size - 1) {
			m_graph->addArc(i, i + 1);
		}
	}
	return success;
}


void Game::destroy()
{
	m_renderer->destroy();
	delete m_renderer;
	m_renderer = nullptr;
}

//** calls update on all game entities*/
void Game::update()
{
	unsigned int currentTime = SDL_GetTicks();//millis since game started
	unsigned int deltaTime = currentTime - lastTime;//time since last update
	if (deltaTime > SCREEN_TICKS_PER_FRAME + 3)
	{
		deltaTime = 0;
	}
}

//** calls render on all game entities*/
void Game::render()
{
	m_renderer->clear(Colour(0,0,0,255));

	GraphNode<Tile *> ** nodes = m_graph->getNodes();
	for (int i = 0; i < m_graph->getCount(); i++) {
		nodes[i]->getVal()->render(m_renderer);
	}

	m_renderer->present();// display the new frame (swap buffers)
}

/** update and render game entities*/
void Game::loop()
{
	int frameNum = 0;

	// Game loop
	while (!m_quit)
	{

		inputManager->ProcessInput();

		update();

		render();

		unsigned int currentTime = SDL_GetTicks();//millis since game started
		// Time since start of frame
		int frameTicks = currentTime - lastTime;

		if (frameTicks < SCREEN_TICKS_PER_FRAME)
		{
			// Wait remaining time before going to next frame
			SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
		}

		//save the curent time for next frame
		lastTime = currentTime;
	}
}

void Game::onEvent(EventListener::Event evt)
{
	if (evt == EventListener::Event::QUIT)
	{
		m_quit = true;
	}
}