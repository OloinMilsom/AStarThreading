#include "Enemy.h"

void Enemy::pathFunc(void * val) {
	// cast data
	auto data = static_cast<std::tuple<Enemy *, Graph<Tile *> *, int> *>(val);
	// wait until signal to recalculate
	SDL_SemWait(std::get<0>(*data)->m_sem);
	// do A*
	std::get<1>(*data)->aStar(std::get<0>(*data)->m_indexPos, std::get<2>(*data), &std::get<0>(*data)->m_path, [](Tile * x, float y) { x->setColour(Colour(0, 0, y)); });
	// notify enemy a new path is ready
	std::get<0>(*data)->m_recalculating = false;
	std::get<0>(*data)->m_funcComplete = true;

	delete data;
}

Enemy::Enemy(int pos) 
	:GameEntity(pos),
	 m_sem(SDL_CreateSemaphore(0)){
	m_path = std::vector<int>();
	m_funcComplete = true;
}

int Enemy::getIndexPos() const {
	return m_indexPos;
}

void Enemy::update(Graph<Tile*> * graph, int size) {
	if (!m_path.empty()) {
		m_indexPos = m_path.back();
		m_path.pop_back();
	}
	else if (!m_recalculating) {
		m_recalculating = true;
		SDL_SemPost(m_sem);
	}
	m_worldPos = graph->getNode(m_indexPos)->getVal()->getRect().pos;
}

void Enemy::render(Renderer * renderer) const {
	renderer->drawWorldFillRect(Rect(m_worldPos, Size(0.5, 0.5)), Colour(255, 0, 0));
}

void Enemy::updatePath(Graph<Tile *> * graph, int size, int playerIndex) {
	if (m_funcComplete) {
		m_funcComplete = false;
		auto data = new std::tuple<Enemy *, Graph<Tile *> *, int>(this, graph, playerIndex);
		ThreadQueue::getInstance()->addJob(pathFunc, data);
	}
}
