#pragma once
#include "BasicTypes.h"
#include "EventListener.h"
#include "Graph.h"
#include "Tile.h"
#include "GameEntity.h"
#include<vector>
#include "ThreadQueue.h"
#include <tuple>

class Enemy : public GameEntity {
private:
	std::vector<int> * m_path; // 0 = w, 1 = a, 2 = s, 3 = d

public:
	Enemy(int pos = 0);
	int getIndexPos() const;
	void update(Graph<Tile *> * graph, int size);
	void render(Renderer * renderer) const;
	void updatePath(Graph<Tile *> * graph, int size, int playerIndex);
};
