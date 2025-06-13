#include <vector>
#include <string>
#include <iostream>
#include <thread>
#include <condition_variable> 
#include <queue>
#include <mutex>
#include <chrono>
#include <ctime>


// Task structure 
struct Tarea { 
    int idSensor; 
    int idTarea; 
    std::string descripcionTarea; 
};

// Global variables 
std::queue<Tarea> taskQueue; 
std::mutex queueMutex; 
std::condition_variable taskAvailable; 
std::mutex consoleMutex; 
int activeSensors = 3; // Number of sensors still generating tasks 
bool allSensorsDone = false;
int globalTaskId = 1;

std::vector<std::string> descripciones = {
    "Transportar materiales al área de ensamblaje",
    "Verificar la calidad de las piezas producidas",
    "Soldar componentes metálicos",
    "Empaquetar productos terminados",
    "Lubricar maquinaria en la línea de producción",
    "Clasificar piezas defectuosas",
    "Cargar materias primas en la tolva de alimentación",
    "Montar subcomponentes electrónicos",
    "Inspeccionar niveles de inventario en estanterías",
    "Paletizar cajas para su envío"
};
// Sensor function 
void sensor(int id, int numTasks) { 
    for (int i = 1; i <= numTasks; ++i) { 
        // Simulate task creation (175ms) 
        std::this_thread::sleep_for(std::chrono::milliseconds(175));
    
     int taskId;
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        taskId = globalTaskId++;
    }

    // Create task
    Tarea tarea{id, taskId, descripciones[rand() % descripciones.size()]};

    // Push task to queue
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        taskQueue.push(tarea);
        {
            std::lock_guard<std::mutex> consoleLock(consoleMutex);
            std::cout << "Sensor " << id << " creó tarea " << taskId << ": " << tarea.descripcionTarea << std::endl;
        }
    }

    // Notify a waiting robot
    taskAvailable.notify_one();
}

// Sensor finished generating tasks
{
    std::lock_guard<std::mutex> lock(queueMutex);
    --activeSensors;
    if (activeSensors == 0) {
        allSensorsDone = true;
        {
            std::lock_guard<std::mutex> consoleLock(consoleMutex);
            std::cout << "Sensor " << id << " finalizó la generación de tareas" << std::endl;
        }
        taskAvailable.notify_all(); // Wake all robots to check termination
    } else {
        std::lock_guard<std::mutex> consoleLock(consoleMutex);
        std::cout << "Sensor " << id << " finalizó la generación de tareas" << std::endl;
    }
}

}

// Robot function 
void robot(int id) { 
    std::srand(std::time(nullptr));
    while (true) { 
        Tarea tarea; bool taskRetrieved = false;
    // Wait for a task or termination condition
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        taskAvailable.wait(lock, [&] {
            return !taskQueue.empty() || (allSensorsDone && taskQueue.empty());
        });

        // Check for termination
        if (taskQueue.empty() && allSensorsDone) {
            break;
        }

        // Retrieve task
        tarea = taskQueue.front();
        taskQueue.pop();
        taskRetrieved = true;
    }

    if (taskRetrieved) {
        // Process task (250ms)
        std::this_thread::sleep_for(std::chrono::milliseconds(250));

        // Report processing
        {
            std::lock_guard<std::mutex> consoleLock(consoleMutex);
            std::cout << "Robot " << id << " procesó tarea " << tarea.idTarea << std::endl;
        }
    }
}

// Robot finished
{
    std::lock_guard<std::mutex> consoleLock(consoleMutex);
    std::cout << "Robot " << id << " finalizó su trabajo" << std::endl;
}

}

int main() { 
    const int numSensors = 3; 
    const int numRobots = 3; 
    const int tasksPerSensor = 10;

    // Create sensor threads
    std::vector<std::thread> sensorThreads;
    for (int i = 0; i < numSensors; ++i) {
        sensorThreads.emplace_back(sensor, i, tasksPerSensor);
    }

    // Create robot threads
    std::vector<std::thread> robotThreads;
    for (int i = 0; i < numRobots; ++i) {
        robotThreads.emplace_back(robot, i);
    }

    // Wait for sensors to finish
    for (auto& thread : sensorThreads) {
        thread.join();
    }

    // Wait for robots to finish
    for (auto& thread : robotThreads) {
        thread.join();
    }

    return 0;

}