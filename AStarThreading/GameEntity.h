#pragma once
#include "BasicTypes.h"
#include "Graph.h"
#include "Tile.h"
#include "Renderer.h"

class GameEntity {
protected:
	int m_indexPos;
	Vector2 m_worldPos;

public:
	GameEntity(int pos = 0) : m_indexPos(pos) { };
	virtual int getIndexPos() const = 0;
	void setWorldPos(Vector2 pos) { m_worldPos = pos; }
	void setIndexPos(int pos) { m_indexPos = pos; }
	virtual void update(Graph<Tile *> * graph, int size) = 0;
	virtual void render(Renderer * renderer) const = 0;
};