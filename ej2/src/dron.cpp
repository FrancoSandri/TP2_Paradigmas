#include "../headers/dron.hpp" 
#include <iostream>

// Global mutex for console output
std::mutex consoleMutex;

Dron::Dron(int id, std::mutex& leftZone, std::mutex& rightZone) : id(id), leftZone(leftZone), rightZone(rightZone) {}

void Dron::takeoff() { 
    {
        std::lock_guard<std::mutex> consoleLock(consoleMutex); 
        std::cout << "Dron " << id << " esperando para despegar..." << std::endl;
    } 

    // Acquire both zones atomically to avoid deadlock
    std::unique_lock<std::mutex> lock1(leftZone, std::defer_lock);
    std::unique_lock<std::mutex> lock2(rightZone, std::defer_lock);
    std::lock(lock1, lock2);

    {
        std::lock_guard<std::mutex> consoleLock(consoleMutex);
        std::cout << "Dron " << id << " despegando..." << std::endl;
    }

    // Simulate 5-second takeoff
    std::this_thread::sleep_for(std::chrono::seconds(5));

    {
        std::lock_guard<std::mutex> consoleLock(consoleMutex);
        std::cout << "Dron " << id << " alcanzó altura de 10m" << std::endl;
    }

// Zones are automatically released when lock1 and lock2 go out of scope
}

void Dron::start() { 
    thread = std::thread(&Dron::takeoff, this); 
}

void Dron::join() { 
    if (thread.joinable()) { 
        thread.join(); 
    } 
}

Dron::~Dron() { 
    join(); 
}
