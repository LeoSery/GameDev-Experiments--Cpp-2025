#include "WorkerPool.h"

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <string>

/**
 * @brief Construct a new Worker Pool:: Worker Pool object
 * 
 * This constructor initializes the worker pool by determining the maximum number of workers
 * based on the hardware concurrency. It then starts the specified number of worker threads
 * and begins the worker pool operation.
 */
WorkerPool::WorkerPool()
{
    const uint32_t maxWorkerNumber = std::max(1u, std::thread::hardware_concurrency() - 1);
    std::cout << "[WorkerPool] Starting with " << maxWorkerNumber << " workers\n";

    for (uint32_t i = 0; i < maxWorkerNumber; i++)
    {
        workersList.emplace_back(std::jthread(&WorkerPool::Work, this, i));
    }

    Start();
}

/**
 * @brief Destroy the Worker Pool:: Worker Pool object
 * 
 * This destructor ensures that the worker pool is properly stopped and cleaned up
 * by calling the Stop() method, which stops all worker threads and clears all jobs.
 */
WorkerPool::~WorkerPool()
{
    Stop();
}

/**
 * @brief Starts the worker pool.
 * 
 * This method sets the isRunning flag to true, indicating that the worker pool is active and ready to process jobs.
 */
void WorkerPool::Start()
{
    isRunning = true;
}

/**
 * @brief Stops the worker pool.
 * 
 * This method stops the worker pool by setting the isRunning flag to false, clearing all jobs,
 * stopping all worker threads, and notifying all waiting threads. It ensures that the worker pool
 * is properly stopped and cleaned up.
 */
void WorkerPool::Stop()
{
    if (!isRunning)
    {
        return;
    }

    isRunning = false;
    ClearAllJobs();
    StopAllWorkers();
    std::cout << "[WorkerPool] Stopped\n";
}

void WorkerPool::WaitForCompletion() const
{
    while (GetPendingJobsCount() > 0)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

/**
 * @brief Adds a new job to the worker pool.
 * 
 * This method adds a new job to the jobs queue and assigns a unique job ID to it.
 * It then notifies one of the waiting worker threads to start processing the job.
 */
void WorkerPool::AddJob(std::function<void()> job)
{
    int jobId = nextJobId++;

    {
        std::scoped_lock lock(jobMutex);
        jobQueue.push({ job, jobId });
        std::cout << "\n";
        std::cout << "[WorkerPool] New Job Added | Queue size: " << std::setw(2) << jobQueue.size() << " | Task ID: " << std::setw(3) << jobId << "\n";
    }
    ConditionalVariable.notify_one();
}

/**
 * @brief Clears all jobs from the jobs queue.
 * 
 * This method removes all pending jobs from the jobs queue by acquiring a lock on the jobMutex
 * and then popping each job from the queue until it is empty.
 */
void WorkerPool::ClearAllJobs()
{
    {
        std::scoped_lock lock(jobMutex);
        while (!jobQueue.empty())
        {
            jobQueue.pop();
        }
    }
}

/**
 * @brief Stops all worker threads.
 * 
 * This method requests all worker threads to stop by calling request_stop() on each worker.
 * It then notifies all waiting threads to ensure they can exit their wait state.
 */
void WorkerPool::StopAllWorkers()
{
    for (auto& worker : workersList)
    {
        worker.request_stop();
    }
    ConditionalVariable.notify_all();
}

/**
 * @brief Checks if the worker pool is currently running.
 * 
 * This method returns the status of the worker pool, indicating whether it is currently active and processing jobs.
 */
bool WorkerPool::IsRunning() const
{
    return isRunning;
}

/**
 * @brief Get the count of pending jobs in the queue.
 * 
 * This method returns the number of jobs that are currently pending in the jobs queue.
 * It acquires a lock on the jobMutex to ensure thread safety while accessing the queue.
 */
int WorkerPool::GetPendingJobsCount() const
{
    std::scoped_lock lock(jobMutex);
    return jobQueue.size();
}

/**
 * @brief Worker thread function.
 * 
 * This method is executed by each worker thread in the pool. It continuously waits for jobs to be added to the queue.
 * When a job is available, it processes the job and then waits for the next job. The method exits when a stop request is received.
 */
void WorkerPool::Work(std::stop_token stopToken, WorkerPool* self, int workerID)
{
    while (!stopToken.stop_requested())
    {
        std::unique_lock<std::mutex> lock(self->jobMutex);

        self->ConditionalVariable.wait(lock, [self]
        {
            return !self->jobQueue.empty();
        });

        if (self->jobQueue.empty()) continue;

        auto [currentJob, jobId] = self->jobQueue.front();
        self->jobQueue.pop();

        self->LogWorkerMessage(jobId, workerID, "Attributed | Queue size: " + std::to_string(self->jobQueue.size()), self->silent);
        lock.unlock();

        self->LogWorkerMessage(jobId, workerID, "Started", self->silent);
        currentJob();
        self->LogWorkerMessage(jobId, workerID, "Completed", self->silent);
    }
}

void WorkerPool::LogWorkerMessage(int jobId, int workerID, const std::string& message, bool silent) const
{
    if (!silent)
    {
        std::cout << "[WorkerPool > " << std::setw(3) << jobId << "] " << message << " (Worker" << workerID << ")\n";
    }
}
