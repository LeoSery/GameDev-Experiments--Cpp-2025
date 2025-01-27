#include "WorkerPool.h"

#include <functional>
#include <iostream>
#include <conio.h>
#include <thread>
#include <vector>
#include <random>
#include <queue>

/**
 * @brief Adds a quick job to the WorkerPool.
 * 
 * This function adds a job to the WorkerPool that prints a hello message.
 */
void LunchQuickTack(WorkerPool& pool, bool silent = false)
{
    pool.AddJob([silent]()
        {
            if (!silent)
            {
                std::cout << "[WorkerPool > Task] Hello from quick job!\n";
            }
        });
}

/**
 * @brief Adds a long job to the WorkerPool.
 * 
 * This function adds a job to the WorkerPool that simulates a long task by
 * sleeping for 3 seconds and then prints a completion message.
 */
void LunchLongTask(WorkerPool& pool, bool silent = false)
{
    pool.AddJob([silent]()
        {
            if (!silent)
            {
                std::cout << "[WorkerPool > Task] Starting long task (3s)...\n";
            }

            std::this_thread::sleep_for(std::chrono::seconds(3));

            if (!silent)
            {
                std::cout << "[WorkerPool > Task] Long task finished!\n";
            }
        });
}

/**
 * @brief Adds a math job to the WorkerPool.
 * 
 * This function adds a job to the WorkerPool that generates two random integers,
 * adds them together, and prints the result.
 */
void LunchMathTask(WorkerPool& pool, std::uniform_int_distribution<>& dis, std::mt19937& gen, bool silent = false)
{
    pool.AddJob([&dis, &gen, silent]()
        {
            int num1 = dis(gen);
            int num2 = dis(gen);

            if (!silent)
            {
                std::cout << "[WorkerPool > Task] " << num1 << " + " << num2 << " = " << (num1 + num2) << "\n";
            }
        });
}

/**
 * @brief Adds a multiple print job to the WorkerPool.
 * 
 * This function adds a job to the WorkerPool that prints a message five times,
 * with a 500 milliseconds delay between each print.
 */
void LunchMultipleQuickTask(WorkerPool& pool)
{
    pool.AddJob([]()
    {
        for (int i = 0; i < 5; i++)
        {
            std::cout << "[WorkerPool > Task] Print " << i + 1 << "/5\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    });
}

/**
 * @brief Adds a random task to the WorkerPool.
 * 
 * This function randomly selects a type of task (quick, long, or math) and adds it to the WorkerPool.
 */
void LaunchRandomTask(WorkerPool& pool, std::uniform_int_distribution<>& dis, std::mt19937& gen, bool silent = false)
{
    std::uniform_int_distribution<> taskDis(1, 3);
    int taskType = taskDis(gen);

    switch (taskType)
    {
    case 1:
        LunchQuickTack(pool, silent);
        break;
    case 2:
        LunchLongTask(pool, silent);
        break;
    case 3:
        LunchMathTask(pool, dis, gen, silent);
        break;
    }
}

/**
 * @brief Launches multiple random tasks in the WorkerPool.
 * 
 * This function adds a specified number of random tasks to the WorkerPool and
 * waits for all tasks to complete. It measures and prints the total execution
 * time and the average time per task.
 */
void LaunchMultipleRandomTasks(WorkerPool& pool, std::uniform_int_distribution<>& dis, std::mt19937& gen, int numTasks)
{
    std::cout << "\n[WorkerPool] Launching batch of " << numTasks << " random tasks...\n";

    auto startTime = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < numTasks; i++)
    {
        LaunchRandomTask(pool, dis, gen, true);
    }

    pool.WaitForCompletion();

    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

    std::cout << "[WorkerPool] Batch completed:\n";
    std::cout << "- Tasks launched: " << numTasks << "\n";
    std::cout << "- Total execution time: " << duration.count() << "ms\n";
    std::cout << "- Average time per task: " << duration.count() / numTasks << "ms\n\n";
}

/**
 * @brief Main function to run the WorkerPool example.
 * 
 * This function initializes a WorkerPool and provides a simple console interface
 * to add different types of jobs to the pool. The user can press keys to add
 * quick jobs, long jobs, random addition jobs, or multiple print jobs. The user
 * can also quit the application by pressing 'q' or 'Q'.
 * 
 * Controls:
 * 1: Quick job (prints hello)
 * 2: Long job (3 seconds)
 * 3: Random addition
 * 4: Multiple prints
 * q: Quit
 */
int main()
{
    WorkerPool pool;

    std::cout << "Controls:\n";
    std::cout << "1: Quick job (prints hello)\n";
    std::cout << "2: Long job (3 seconds)\n";
    std::cout << "3: Random addition\n";
    std::cout << "4: Multiple prints\n";
    std::cout << "5: Multiple random tasks\n";
    std::cout << "q: Quit\n\n";

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);

    while (pool.IsRunning())
    {
        if (_kbhit())
        {
            char key = _getch();
            switch (key)
            {
            case '1':
                LunchQuickTack(pool);
                break;

            case '2':
                LunchLongTask(pool);
                break;

            case '3':
                LunchMathTask(pool, dis, gen);
                break;

            case '4':
                LunchMultipleQuickTask(pool);
                break;

            case '5':
                LaunchMultipleRandomTasks(pool, dis, gen, 100);
                break;

            case 'q':
            case 'Q':
                pool.Stop();
                break;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    return 0;
}
