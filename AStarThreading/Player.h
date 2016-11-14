#pragma once
#include "BasicTypes.h"
#include "EventListener.h"
#include "Graph.h"
#include "Tile.h"
#include<vector>

class Player : public EventListener {
private:
	int m_indexPos;
	Vector2 m_worldPos;
	std::vector<bool> m_keydowns; // 0 = w, 1 = a, 2 = s, 3 = d

public:
	Player(int pos = 0);
	int getIndexPos() const;
	void setWorldPos(Vector2 pos);
	void update(Graph<Tile *> const * graph, int size);
	void render(Renderer * renderer) const;
	void onEvent(Event e);
};