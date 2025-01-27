# WorkerPool

A lightweight C++ thread pool implementation for managing concurrent task execution.

![Terminal Demo](./ReadmeContent/GameScreenshots/Demo.jpg)

## Overview
Thread pool system that automatically manages worker threads and distributes tasks. Built with modern C++ features, it offers a simple interface for adding and executing jobs concurrently.

## Features
- Automatic thread pool sizing based on hardware
- Thread-safe job queue
- Support for various task types
- Real-time task monitoring

## Usage
```cpp
// Create pool
WorkerPool pool;

// Quick job
pool.AddJob([]() {
    std::cout << "Hello from quick job!\n";
});

// Math task (random addition)
pool.AddJob([&dis, &gen]() {
    int num1 = dis(gen);
    int num2 = dis(gen);
    std::cout << num1 << " + " << num2 << " = " << (num1 + num2) << "\n";
});

// Multiple prints task
pool.AddJob([]() {
    for (int i = 0; i < 5; i++) {
        std::cout << "Print " << i + 1 << "/5\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
});
```