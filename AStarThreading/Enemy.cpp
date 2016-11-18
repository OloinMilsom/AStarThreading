#include "Enemy.h"

Enemy::Enemy(int pos) 
	:GameEntity(pos) {
	m_path = new std::vector<Tile *>();
}

int Enemy::getIndexPos() const {
	return m_indexPos;
}

void Enemy::update(Graph<Tile*> * graph, int size) {
	m_worldPos = graph->getNode(m_indexPos)->getVal()->getRect().pos;
	graph->aStar(m_indexPos, 0, m_path);
}

void Enemy::render(Renderer * renderer) const {
	renderer->drawWorldFillRect(Rect(m_worldPos, Size(0.5, 0.5)), Colour(255, 0, 0));
}
