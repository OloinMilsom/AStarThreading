#include "InputManager.h"
#include <iostream>
using namespace std;

InputManager::InputManager()
{

}


InputManager::~InputManager()
{

}

void InputManager::AddListener(EventListener::Event evt, EventListener *listener)
{
	if (listeners.find(evt) == listeners.end())
	{ //event not in map add it.
		listeners[evt] = new std::vector<EventListener*>();
	}

	listeners[evt]->push_back(listener);

}

//Send the events to interested objects
void InputManager::Dispatch(EventListener::Event evt)
{
	if (listeners.find(evt) != listeners.end())
	{
		for (auto const &listener : *listeners[evt]) { //go through all listeners for this event
			listener->onEvent(evt); //Call on event for the listener
		}
	}
}

//Gnereate events
void InputManager::ProcessInput()
{
	SDL_Event e;

	while (SDL_PollEvent(&e) != 0)
	{
		switch (e.type) {
			/* Keyboard event */
		case SDL_KEYDOWN:
			switch (e.key.keysym.sym)
			{
			case SDLK_w:
				Dispatch(EventListener::Event::W_KEY_DOWN);
				break;
			case SDLK_a:
				Dispatch(EventListener::Event::A_KEY_DOWN);
				break;
			case SDLK_s:
				Dispatch(EventListener::Event::S_KEY_DOWN);
				break;
			case SDLK_d:
				Dispatch(EventListener::Event::D_KEY_DOWN);
				break;
			}
			break;

		case SDL_KEYUP:
			switch (e.key.keysym.sym)
			{
			case SDLK_w:
				Dispatch(EventListener::Event::W_KEY_UP);
				break;
			case SDLK_a:
				Dispatch(EventListener::Event::A_KEY_UP);
				break;
			case SDLK_s:
				Dispatch(EventListener::Event::S_KEY_UP);
				break;
			case SDLK_d:
				Dispatch(EventListener::Event::D_KEY_UP);
				break;
			}
			break;

		case SDL_QUIT:
			Dispatch(EventListener::Event::QUIT);
			break;

		default:
			break;
		}
	}
}
