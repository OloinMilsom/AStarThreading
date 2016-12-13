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
	std::vector<int> m_path;
	SDL_sem * m_sem;
	SDL_mutex * m_lock;
	bool m_recalculating;
	bool m_funcComplete;
	static void pathFunc(void * val);
	std::tuple<Graph<Tile *> *, int> m_threadData;
public:
	Enemy(int pos = 0);
	int getIndexPos() const;
	SDL_sem * getSem() const;
	void update(Graph<Tile *> * graph, int size);
	void render(Renderer * renderer) const;
	void updatePath(Graph<Tile *> * graph, int size, int playerIndex);
};
