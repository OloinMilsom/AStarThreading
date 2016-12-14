#include "ThreadQueue.h"
#include <iostream>

ThreadQueue * ThreadQueue::m_instance = nullptr;

ThreadQueue::ThreadQueue() :m_sem(SDL_CreateSemaphore(0)), m_stopSem(SDL_CreateSemaphore(1)), m_lock(SDL_CreateMutex()) {

}

int ThreadQueue::worker(void * ptr)
{
	srand(time(0));
	while (m_instance->m_workersOpen) {
		// normally this semaphore operation should do nothing if undisturbed
		// however elsewhere the semaphore can be used to stop this from operating normally
		// thus this semaphore can be used to stop all worker threads and restart them
		std::cout << "stopped" << std::endl;
		SDL_SemWait(m_instance->m_stopSem);
		std::cout << "started" << std::endl;
		SDL_SemPost(m_instance->m_stopSem);

		SDL_SemWait(m_instance->m_sem);

		std::cout << "job started" << std::endl;

		std::pair<void(*)(void *), void *> job = m_instance->consumeJob();

		job.first(job.second);
	}
	return 0;
}

ThreadQueue * ThreadQueue::getInstance() {
	if (m_instance == nullptr)	{
		m_instance = new ThreadQueue();
	}
	return m_instance;
}

void ThreadQueue::createWorkers()
{
	int numWorkers = std::thread::hardware_concurrency() - 1;
	m_workersOpen = true;

	for (int i = 0; i < numWorkers; i++)
	{
		m_workerPool.push_back(SDL_CreateThread(worker, "GenericWorker"  + 1, (void*)NULL));
	}
}

std::pair<void(*)(void *), void *> ThreadQueue::consumeJob()
{
	SDL_LockMutex(m_lock);
	std::pair<void (*)(void *), void *> job = m_jobQueue.front();
	m_jobQueue.pop();
	SDL_UnlockMutex(m_lock);
	return job;
}

void ThreadQueue::addJob(void(*f)(void * x), void * x)
{
	SDL_LockMutex(m_lock);
	m_jobQueue.push(std::make_pair(f, x));
	SDL_UnlockMutex(m_lock);
	SDL_SemPost(m_sem);
	std::cout << "job added" << std::endl;
}

void ThreadQueue::stop() {
	// stop all worker threads
	SDL_SemWait(m_stopSem);
}

void ThreadQueue::start() {
	// start all worker threads
	SDL_SemPost(m_stopSem);
}

bool ThreadQueue::reset()
{
	if (SDL_SemValue(m_stopSem) == 0) {
		SDL_LockMutex(m_lock);
		while (!m_jobQueue.empty()) {
			SDL_SemWait(m_instance->m_sem);
			m_jobQueue.pop();
		}
		SDL_UnlockMutex(m_lock);
		std::cout << "reset" << std::endl;
		return true;
	}
	else {
		return false;
	}
}
