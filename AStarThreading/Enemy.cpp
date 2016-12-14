#include "Enemy.h"

void Enemy::pathFunc(void * val) {	
	// cast data
	auto enemy = static_cast<Enemy *>(val);
	
	SDL_SemWait(enemy->m_deleteSem);

	// wait until signal to recalculate
	SDL_SemWait(enemy->m_sem);

	std::get<0>(enemy->m_threadData)->aStar(enemy->m_indexPos, std::get<1>(enemy->m_threadData), &enemy->m_path, [](Tile * x, float y) { x->setColour(Colour(0, 0, y)); }, enemy->m_lock);
	// notify enemy a new path is ready

	SDL_LockMutex(enemy->m_lock);
	enemy->m_recalculating = false;
	enemy->m_funcComplete = true;
	SDL_UnlockMutex(enemy->m_lock);

	SDL_SemPost(enemy->m_deleteSem);
}

Enemy::Enemy(int pos) 
	:GameEntity(pos),
	 m_sem(SDL_CreateSemaphore(0)),
	 m_deleteSem(SDL_CreateSemaphore(1)),
	 m_lock(SDL_CreateMutex()) {
	m_path = std::vector<int>();
	m_funcComplete = true;
}

Enemy::~Enemy() {
	SDL_SemPost(m_sem);
	SDL_SemWait(m_deleteSem);
	SDL_DestroyMutex(m_lock);
	SDL_DestroySemaphore(m_sem);
}

int Enemy::getIndexPos() const {
	return m_indexPos;
}

SDL_sem * Enemy::getSem() const {
	return m_sem;
}

void Enemy::update(Graph<Tile*> * graph, int size) {
	SDL_LockMutex(m_lock);
	if (!m_path.empty()) {
		m_indexPos = m_path.back();
		m_path.pop_back();
	}
	else if (!m_recalculating) {
		m_recalculating = true;
		SDL_SemPost(m_sem);
	}
	SDL_UnlockMutex(m_lock);

	m_worldPos = graph->getNode(m_indexPos)->getVal()->getRect().pos;
}

void Enemy::render(Renderer * renderer) const {
	renderer->drawWorldFillRect(Rect(m_worldPos, Size(0.5, 0.5)), Colour(255, 0, 0));
}

void Enemy::updatePath(Graph<Tile *> * graph, int size, int playerIndex) {
	if (m_funcComplete && playerIndex != m_indexPos) {
		SDL_LockMutex(m_lock);
		m_funcComplete = false;
		SDL_UnlockMutex(m_lock);

		m_threadData = std::tuple<Graph<Tile *> *, int>(graph, playerIndex);
		ThreadQueue::getInstance()->addJob(pathFunc, this);
	}
}
