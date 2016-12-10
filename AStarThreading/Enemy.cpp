#include "Enemy.h"

void pathFunc(void * val) {
	// 0 = path
	auto data = static_cast<std::tuple<Graph<Tile *> *, int, int, std::vector<int> *, bool *> *>(val);
	std::get<0>(*data)->aStar(std::get<1>(*data), std::get<2>(*data), std::get<3>(*data), [](Tile * x, float y) { x->setColour(Colour(y, y, y)); });
	*std::get<4>(*data) = true;
	delete data;
}

Enemy::Enemy(int pos) 
	:GameEntity(pos) {
	m_path = std::vector<int>();
	m_newPath = std::vector<int>();
	m_newPathReady = true;
}

int Enemy::getIndexPos() const {
	return m_indexPos;
}

void Enemy::update(Graph<Tile*> * graph, int size) {	
	if (!m_path.empty()) {
		m_indexPos = m_path.back();
		m_path.pop_back();
	}
	m_worldPos = graph->getNode(m_indexPos)->getVal()->getRect().pos;
}

void Enemy::render(Renderer * renderer) const {
	renderer->drawWorldFillRect(Rect(m_worldPos, Size(0.5, 0.5)), Colour(255, 0, 0));
}

void Enemy::updatePath(Graph<Tile *> * graph, int size, int playerIndex) {
	if (m_newPathReady) {
		m_path = m_newPath;
		m_newPathReady = false;
		m_newPath.clear();
		auto data = new std::tuple<Graph<Tile *> *, int, int, std::vector<int> *, bool *>(graph, m_indexPos, playerIndex, &m_newPath, &m_newPathReady);
		ThreadQueue::getInstance()->addJob(pathFunc, data);
	}
}
