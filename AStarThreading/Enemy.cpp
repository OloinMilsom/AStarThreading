#include "Enemy.h"

Enemy::Enemy(int pos) 
	:GameEntity(pos) {
	m_path = new std::vector<int>();
}

int Enemy::getIndexPos() const {
	return m_indexPos;
}

void Enemy::update(Graph<Tile*> * graph, int size) {	
	if (!m_path->empty())
		m_indexPos = m_path->back();
	m_worldPos = graph->getNode(m_indexPos)->getVal()->getRect().pos;
}

void Enemy::render(Renderer * renderer) const {
	renderer->drawWorldFillRect(Rect(m_worldPos, Size(0.5, 0.5)), Colour(255, 0, 0));
}

void Enemy::updatePath(Graph<Tile *> * graph, int size, int playerIndex) {
	m_path->clear();
	graph->aStar(m_indexPos, playerIndex, m_path, [](Tile * x, float y) { x->setColour(Colour(y, y, y)); });
}
