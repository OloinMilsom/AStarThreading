#pragma once

#include<map>
#include<vector>

#include "EventListener.h"
#include "SDL.h"

/*Convert input into logical events*/
class InputManager
{
private:
	//dictionary holding a list of litener objs for each event type

	std::map<
		EventListener::Event,
		std::vector<EventListener*>* //pointer to vec of eventlisteners
	> listeners;

public:
	InputManager();
	~InputManager();

	void AddListener(EventListener::Event, EventListener*);
	void Dispatch(EventListener::Event);

	void ProcessInput();
};