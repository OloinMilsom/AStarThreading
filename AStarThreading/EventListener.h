#pragma once

/**Used as an interface for the event dispatcher*/
/*Objects wanting to receive events must inherit from this*/

class EventListener
{
public:
	//list of possible events
	//add your own events here
	enum Event {
		W_KEY_DOWN,
		A_KEY_DOWN,
		S_KEY_DOWN,
		D_KEY_DOWN,
		W_KEY_UP,
		A_KEY_UP,
		S_KEY_UP,
		D_KEY_UP,
		NUM1_KEY_DOWN,
		NUM2_KEY_DOWN,
		NUM3_KEY_DOWN,
		QUIT
	};

	virtual void onEvent(Event) = 0;
};

