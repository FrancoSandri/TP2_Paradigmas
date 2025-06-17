#include "../headers/dron.hpp" 
#include <iostream>

// Mutex global para sincronizar el acceso a la consola
std::mutex consoleMutex;

Dron::Dron(int id, std::mutex& leftZone, std::mutex& rightZone) : id(id), leftZone(leftZone), rightZone(rightZone) {}

void Dron::takeoff() { 
    {
        std::lock_guard<std::mutex> consoleLock(consoleMutex); 
        std::cout << "Dron " << id << " esperando para despegar..." << std::endl;
    } 

    // Adquiere locks para ambas zonas para evitar deadlocks
    // Utilizamos std::defer_lock para evitar deadlocks al adquirir los locks en orden
    // Primero bloqueamos la zona izquierda y luego la derecha para asegurar un orden consistente
    // Esto es para evitar que dos drones intenten despegar al mismo tiempo en zonas adyacentes.
    std::unique_lock<std::mutex> lock1(leftZone, std::defer_lock);
    std::unique_lock<std::mutex> lock2(rightZone, std::defer_lock);
    std::lock(lock1, lock2);

    {
        std::lock_guard<std::mutex> consoleLock(consoleMutex);
        std::cout << "Dron " << id << " despegando..." << std::endl;
    }

    // Simula el tiempo de despegue
    std::this_thread::sleep_for(std::chrono::seconds(5));

    {
        std::lock_guard<std::mutex> consoleLock(consoleMutex);
        std::cout << "Dron " << id << " alcanzó altura de 10m" << std::endl;
    }

// Las zonas se desbloquean automáticamente al salir del scope de los unique_lock.
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
