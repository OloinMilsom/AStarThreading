#pragma once

#include <SDL_mutex.h>
#include <SDL_thread.h>
#include <queue>
#include <functional>
#include <time.h>
#include <thread>

class ThreadQueue {
private:
	ThreadQueue();
	static ThreadQueue * m_instance;
	static int worker(void * ptr);
	SDL_mutex * m_lock;
	SDL_cond * m_condition;
	std::queue<std::pair<void (*)(void *), void *>> m_jobQueue;
	std::vector<SDL_Thread*> m_workerPool;

public:
	static ThreadQueue * getInstance();
	void createWorkers();
	std::pair<void(*)(void *), void *> consumeJob();
	void addJob(void (*f)(void * x), void * x);
};

