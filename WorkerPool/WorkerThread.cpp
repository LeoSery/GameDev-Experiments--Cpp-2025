#include "WorkerThread.h"
#include "WorkerPool.h"

WorkerThread::WorkerThread()
{
	workingConditionVariable = nullptr;
	targetJobQueue = nullptr;
	thread = std::jthread();
}

WorkerThread::WorkerThread(std::condition_variable* CV, std::queue<std::function<void()>>* workingQueue)
	: workingConditionVariable(CV), targetJobQueue(workingQueue)
{
	thread = std::jthread(&WorkerThread::Work, this);
}

WorkerThread::~WorkerThread()
{
	thread.request_stop();
}

void WorkerThread::Work(std::stop_token stopToken)
{
	while (!stopToken.stop_requested())
	{
		workingConditionVariable->wait(jobLock, [this] { return !targetJobQueue->empty(); });

		std::function<void()> currentJob;
		{
			std::scoped_lock lock(jobMutex);
			currentJob = targetJobQueue->front();
			targetJobQueue->pop();
		}

		currentJob();
	}
}

std::jthread& WorkerThread::GetThread()
{
	return thread;
}
