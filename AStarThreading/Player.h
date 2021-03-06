#pragma once
#include "BasicTypes.h"
#include "EventListener.h"
#include "Graph.h"
#include "Tile.h"
#include "GameEntity.h"
#include<vector>

class Player : public EventListener, public GameEntity {
private:
	std::vector<bool> m_keydowns; // 0 = w, 1 = a, 2 = s, 3 = d

public:
	Player(int pos = 0);
	int getIndexPos() const;
	void update(Graph<Tile *> * graph, int size);
	void render(Renderer * renderer) const;
	void onEvent(Event e);
};