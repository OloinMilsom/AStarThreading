#include "Game.h"
#include <iostream>

const int SCREEN_FPS = 100;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

Game::Game()
{
	inputManager = new InputManager();
	m_quit = false;
}

Game::~Game()
{

}

bool Game::init()
{
	Size winSize(800, 600);

	//creates our renderer, which looks after drawing and the window
	renderer.init(winSize, "Thomas Was Alone");

	//set up the viewport
	//we want the vp centred on origin and 20 units wide
	float aspectRatio = winSize.w / winSize.h;
	Size vpSize(40, 40 / aspectRatio);
	Vector2 vpBottomLeft(-vpSize.w / 2, -vpSize.h / 2);

	Rect vpRect(vpBottomLeft, vpSize);
	renderer.setViewPort(vpRect);

	lastTime = SDL_GetTicks();

	inputManager->AddListener(EventListener::Event::QUIT, this);

	return true;
}


void Game::destroy()
{
	renderer.destroy();
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
	renderer.clear(Colour(0,0,0,255));
	renderer.present();// display the new frame (swap buffers)
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