#include "Game.h"
#include <iostream>

const int SCREEN_FPS = 100;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

Game::Game()
{
	inputManager = new InputManager();
	m_player = new Player;
	m_quit = false;
	m_vpWidth = 100;
	m_noOfEnemies = 1;
	m_graph = new Graph<Tile *>(m_vpWidth * m_vpWidth, &Tile::manhattanDistance);
}

Game::~Game()
{

}

bool Game::init()
{
	srand(time(0));
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
	for (int i = 0; i < m_noOfEnemies; i++) {
		m_enemies.push_back(new Enemy(rand() % (m_vpWidth * m_vpWidth)));
		if (m_graph->getNode(m_enemies[i]->getIndexPos())->getVal()->getIsWall()) {
			m_enemies[i]->setIndexPos(m_enemies[i]->getIndexPos() + 1);
		}
	}

	inputManager->AddListener(EventListener::Event::QUIT, this);
	inputManager->AddListener(EventListener::Event::W_KEY_DOWN, m_player);
	inputManager->AddListener(EventListener::Event::A_KEY_DOWN, m_player);
	inputManager->AddListener(EventListener::Event::S_KEY_DOWN, m_player);
	inputManager->AddListener(EventListener::Event::D_KEY_DOWN, m_player);
	inputManager->AddListener(EventListener::Event::W_KEY_UP, m_player);
	inputManager->AddListener(EventListener::Event::A_KEY_UP, m_player);
	inputManager->AddListener(EventListener::Event::S_KEY_UP, m_player);
	inputManager->AddListener(EventListener::Event::D_KEY_UP, m_player);

	return true;
}

bool Game::initGraph()
{
	int size = m_vpWidth;
	bool success = true;
	Colour dark(159, 129, 112);
	Colour light(245, 245, 220);
	Colour grey(64, 64, 64);

	std::vector<int> walls;
	int touchingWalls = 1;
	int totalWalls = 3;
	//setup walls
	for (int i = 0; i < totalWalls; i++)	{
		int x = (i+1) * size / (totalWalls + 1);
		int length = rand() % (size / 5) + (3 * size / 5);
		if (rand() % 2 == 0) {
			for (int j = 0; j < length; j++) {
				walls.push_back(x + j * size);
			}
		}
		else {
			for (int j = size - length; j < size; j++) {
				walls.push_back(x + j * size);
			}
		}
		
	}

	for (int i = 0; i < size * size; i++) {
		Tile * tile = new Tile(Rect(i % size, i / size, 1, 1), std::find(walls.begin(), walls.end(), i) != walls.end());
		if (tile->getIsWall()) {
			tile->setColour(grey);
		}
		else {
			(i + (i / size) % 2) % 2 == 0 ? tile->setColour(light) : tile->setColour(dark);
		}
		success &= m_graph->addNode(tile, i);
	}
	std::vector<GraphNode<Tile *> *> nodes = m_graph->getNodes();
	for (int i = 0; i < size * size; i++) {
		if (!nodes[i]->getVal()->getIsWall()) {
			if (i / size != 0 && !nodes[i - size]->getVal()->getIsWall()) {
				m_graph->addArc(i, i - size);
			}
			if (i / size != size - 1 && !nodes[i + size]->getVal()->getIsWall()) {
				m_graph->addArc(i, i + size);
			}
			if (i % size != 0 && !nodes[i - 1]->getVal()->getIsWall()) {
				m_graph->addArc(i, i - 1);
			}
			if (i % size != size - 1 && !nodes[i + 1]->getVal()->getIsWall()) {
				m_graph->addArc(i, i + 1);
			}
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

	/*auto p = new std::vector<Tile *>();
	m_graph->aStar(0, 45, p);*/

	m_player->update(m_graph, m_vpWidth);
	for (int i = 0; i < m_enemies.size(); i++) {
		m_enemies[i]->update(m_graph, m_vpWidth);
	}

	if (deltaTime > SCREEN_TICKS_PER_FRAME + 3)
	{
		deltaTime = 0;
	}
}

//** calls render on all game entities*/
void Game::render()
{
	m_renderer->clear(Colour(0,0,0,255));

	std::vector<GraphNode<Tile *> *> nodes = m_graph->getNodes();
	for (int i = 0; i < m_graph->getCount(); i++) {
		nodes[i]->getVal()->render(m_renderer);
	}

	m_player->render(m_renderer);

	for (int i = 0; i < m_enemies.size(); i++) {
		m_enemies[i]->render(m_renderer);
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