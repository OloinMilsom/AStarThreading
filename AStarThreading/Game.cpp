#include "Game.h"
#include <iostream>

const int SCREEN_FPS = 100;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

Game::Game()
{
	inputManager = new InputManager();
	ThreadQueue::getInstance()->createWorkers();
	m_player = new Player;
	m_quit = false;
	m_vpWidth = 100;
	m_noOfEnemies = 50;
	m_graph = new Graph<Tile *>(m_vpWidth * m_vpWidth, &Tile::manhattanDistance);
	m_totalWalls = 6;
	m_touchingWalls = 2;
}

Game::~Game()
{

}

bool Game::init()
{
	srand(time(0));
	m_screenSize = Size(1000, 1000);

	//creates our renderer, which looks after drawing and the window
	m_renderer = new Renderer();
	m_renderer->init(m_screenSize, "AStarThreading");
	m_renderer->setViewRect(Rect(0, 0, 1000, 1000));

	//set up the viewport
	//we want the vp centred on origin and 20 units wide
	float aspectRatio = m_screenSize.w / m_screenSize.h;
	Size vpSize(m_vpWidth, m_vpWidth / aspectRatio);
	Vector2 vpBottomLeft(-vpSize.w / 2, -vpSize.h / 2);

	Rect vpRect(vpBottomLeft, vpSize);
	m_renderer->setViewPort(vpRect);

	lastTime = SDL_GetTicks();

	initGraph();
	initEnemies();

	inputManager->AddListener(EventListener::Event::QUIT, this);
	inputManager->AddListener(EventListener::Event::NUM1_KEY_DOWN, this);
	inputManager->AddListener(EventListener::Event::NUM2_KEY_DOWN, this);
	inputManager->AddListener(EventListener::Event::NUM3_KEY_DOWN, this);
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

void Game::reinitialise()
{
	ThreadQueue::getInstance()->stop();

	std::cout << "resetting" << std::endl;

	for (int i = 0; i < m_enemies.size(); i++)
	{
		delete m_enemies[i];
	}
	m_enemies.clear();
	delete m_graph;
	m_graph = new Graph<Tile *>(m_vpWidth * m_vpWidth, &Tile::manhattanDistance);

	initGraph();
	initEnemies();
	m_player->setIndexPos(rand() % (m_vpWidth * m_vpWidth));
	if (m_graph->getNode(m_player->getIndexPos())->getVal()->getIsWall()) {
		m_player->setIndexPos(m_player->getIndexPos() + 1);
	}

	float aspectRatio = m_screenSize.w / m_screenSize.h;
	Size vpSize(m_vpWidth, m_vpWidth / aspectRatio);
	Vector2 vpBottomLeft(-vpSize.w / 2, -vpSize.h / 2);

	Rect vpRect(vpBottomLeft, vpSize);
	m_renderer->setViewPort(vpRect);

	ThreadQueue::getInstance()->reset();
	ThreadQueue::getInstance()->start();
}

void Game::randomiseConditions() {
	int r = rand() % 2; // should be 3, however the max settings are too slow without A* optimisation, may add optimisation in future for myself
	if (r == 0)
	{
		m_vpWidth = 30;
		m_noOfEnemies = 5;
		m_totalWalls = 3;
		m_touchingWalls = 1;
		reinitialise();
	}
	else if (r == 1)
	{
		m_vpWidth = 100;
		m_noOfEnemies = 50;
		m_totalWalls = 6;
		m_touchingWalls = 2;
		reinitialise();
	}
	else if (r == 2)
	{
		m_vpWidth = 1000;
		m_noOfEnemies = 500;
		m_totalWalls = 18;
		m_touchingWalls = 4;
		reinitialise();
	}
}

bool Game::initGraph()
{
	int size = m_vpWidth;
	bool success = true;
	Colour dark(159, 129, 112);
	Colour light(245, 245, 220);
	Colour grey(64, 64, 64);

	std::vector<bool> isTouching(m_totalWalls, false);
	for (int i = 0; i < m_touchingWalls; i++)
	{
		isTouching[i] = true;
	}
	std::random_shuffle(isTouching.begin(), isTouching.end());

	std::vector<int> walls;
	//setup walls
	for (int i = 0; i < m_totalWalls; i++) {
		int x = (i + 1) * size / (m_totalWalls + 1) + rand() % (size / 10) - (size / 20);
		if (isTouching[i]) {
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
		else
		{
			int length = rand() % (size / 5) + (2 * size / 5);
			int start = rand() % (size / 5) + (size / 5);
			for (int j = start; j < start + length; j++) {
				walls.push_back(x + j * size);
			}
		}
	}

	for (int i = 0; i < size * size; i++) {
		Tile * tile = new Tile(Rect(i % size, i / size, 1, 1), std::find(walls.begin(), walls.end(), i) != walls.end());
		if (tile->getIsWall()) {
			tile->setColour(grey);
			tile->setOriginalColour(grey);
		}
		else {
			(i + (i / size) % 2) % 2 == 0 ? tile->setColour(light) : tile->setColour(dark);
			(i + (i / size) % 2) % 2 == 0 ? tile->setOriginalColour(light) : tile->setOriginalColour(dark);
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

bool Game::initEnemies()
{
	int spawnZoneTopLeft = rand() % (m_vpWidth - m_noOfEnemies) + m_vpWidth * (rand() % (m_vpWidth - m_noOfEnemies));
	for (int i = 0; i < m_noOfEnemies; i++) {
		m_enemies.push_back(new Enemy(spawnZoneTopLeft + rand() % m_noOfEnemies + m_vpWidth * (rand() % m_noOfEnemies)));
		if (m_graph->getNode(m_enemies[i]->getIndexPos())->getVal()->getIsWall()) {
			m_enemies[i]->setIndexPos(m_enemies[i]->getIndexPos() + 1);
		}
	}

	return true;
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

	m_player->update(m_graph, m_vpWidth);

	bool enemiesAlive = false;
	for (int i = 0; i < m_enemies.size(); i++) {
		m_enemies[i]->updatePath(m_graph, m_vpWidth, m_player->getIndexPos());
		m_enemies[i]->update(m_graph, m_vpWidth);
		enemiesAlive |= m_enemies[i]->getAlive();
	}
	if (!enemiesAlive) {
		randomiseConditions();
		reinitialise();
	}

	if (deltaTime > SCREEN_TICKS_PER_FRAME + 3)
	{
		deltaTime = 0;
	} 
}

//** calls render on all game entities*/
void Game::render()
{
	m_renderer->clear(Colour(0, 0, 0, 255));

	std::vector<GraphNode<Tile *> *> nodes = m_graph->getNodes();
	for (int i = 0; i < m_graph->getCount(); i++) {
		nodes[i]->getVal()->render(m_renderer);
		//nodes[i]->getVal()->resetColour();
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
	else if(evt == EventListener::Event::NUM1_KEY_DOWN)
	{
		m_vpWidth = 30;
		m_noOfEnemies = 5;
		m_totalWalls = 3;
		m_touchingWalls = 1;
		reinitialise();
	}
	else if (evt == EventListener::Event::NUM2_KEY_DOWN)
	{		
		m_vpWidth = 100;
		m_noOfEnemies = 50;
		m_totalWalls = 6;
		m_touchingWalls = 2;
		reinitialise();
	}
	else if (evt == EventListener::Event::NUM3_KEY_DOWN)
	{
		m_vpWidth = 1000;
		m_noOfEnemies = 500;
		m_totalWalls = 18;
		m_touchingWalls = 4;
		reinitialise();
	}
}