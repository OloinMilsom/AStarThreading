#include "ThreadQueue.h"
#include <iostream>

ThreadQueue * ThreadQueue::m_instance = nullptr;

ThreadQueue::ThreadQueue() :m_sem(SDL_CreateSemaphore(0)), m_lock(SDL_CreateMutex())
{

}

int ThreadQueue::worker(void * ptr)
{
	srand(time(0));
	while (true) {
		SDL_SemWait(m_instance->m_sem);

		std::pair<void(*)(void *), void *> job = m_instance->consumeJob();

		job.first(job.second);
	}
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
}
