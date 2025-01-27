#pragma once

#include <condition_variable>
#include <functional>
#include <thread>
#include <vector>
#include <atomic>
#include <queue>
#include <mutex>

class WorkerPool
{
public:

    //////// CONSTRUCTOR ////////
    WorkerPool();
    ~WorkerPool();

	//////// DELETED METHODS ////////
    WorkerPool(const WorkerPool&) = delete;
    WorkerPool& operator=(const WorkerPool&) = delete;

	//////// METHODS ////////
    //// Init
    void Start();
    void Stop();
    void WaitForCompletion() const;

	//// Jobs
    void AddJob(std::function<void()> job);
    void ClearAllJobs();

    //// Helpers
    [[nodiscard]] bool IsRunning() const;
    [[nodiscard]] int GetPendingJobsCount() const;

private:

	//////// METHODS ////////
	//// Worker
    static void Work(std::stop_token stopToken, WorkerPool* self, int workerID);
    void LogWorkerMessage(int jobId, int workerID, const std::string& message, bool silent = false) const;
    void StopAllWorkers();

    //////// FIELDS ////////
	//// members
    std::atomic<bool> isRunning{ false };
    std::vector<std::jthread> workersList;
    std::atomic<int> nextJobId{ 0 };
    bool silent = false;

	//// static
    mutable std::mutex jobMutex;
    std::queue<std::pair<std::function<void()>, int>> jobQueue;
    std::condition_variable ConditionalVariable;
};