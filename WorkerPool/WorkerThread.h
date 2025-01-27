//#pragma once
//
//#include <condition_variable>
//#include <functional>
//#include <thread>
//#include <queue>
//
//class WorkerThread
//{
//public:
//	WorkerThread();
//	WorkerThread(std::condition_variable* CV, std::queue<std::function<void()>>* workingQueue);
//	~WorkerThread();
//
//	void Work(std::stop_token stoken);
//
//	std::jthread& GetThread();
//
//private:
//	std::jthread thread;
//	std::condition_variable* workingConditionVariable;
//	std::queue<std::function<void()>>* targetJobQueue;
//	std::unique_lock <std::mutex> jobLock;
//};
