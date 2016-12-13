#include "Enemy.h"

void Enemy::pathFunc(void * val) {
	// cast data
	auto enemy = static_cast<Enemy *>(val);
	// wait until signal to recalculate
	SDL_SemWait(enemy->m_sem);
	// do A*

	std::get<0>(enemy->m_threadData)->aStar(enemy->m_indexPos, std::get<1>(enemy->m_threadData), &enemy->m_path, [](Tile * x, float y) { x->setColour(Colour(0, 0, y)); });
	// notify enemy a new path is ready
	enemy->m_recalculating = false;
	enemy->m_funcComplete = true;
}

Enemy::Enemy(int pos) 
	:GameEntity(pos),
	 m_sem(SDL_CreateSemaphore(0)),
	 m_lock(SDL_CreateMutex()){
	m_path = std::vector<int>();
	m_funcComplete = true;
}

int Enemy::getIndexPos() const {
	return m_indexPos;
}

SDL_sem * Enemy::getSem() const {
	return m_sem;
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
		m_threadData = std::tuple<Graph<Tile *> *, int>(graph, playerIndex);
		ThreadQueue::getInstance()->addJob(pathFunc, this);
	}
}
