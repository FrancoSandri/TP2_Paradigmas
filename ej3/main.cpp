#include <vector>
#include <string>
#include <iostream>
#include <thread>
#include <condition_variable> 
#include <queue>
#include <mutex>
#include <chrono>
#include <ctime>


// Struct de una tarea
struct Tarea { 
    int idSensor; 
    int idTarea; 
    std::string descripcionTarea; 
};

// Variables globales
std::queue<Tarea> taskQueue; 
std::mutex queueMutex; 
std::condition_variable taskAvailable; 
std::mutex consoleMutex; 
int activeSensors = 3; // Numero de sensores activos 
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
        // Simulamos una tarea (175ms) 
        std::this_thread::sleep_for(std::chrono::milliseconds(175));
    
     int taskId;
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        taskId = globalTaskId++;
    }

    // Creamos una tarea con un id unico y una descripcion aleatoria
    Tarea tarea{id, taskId, descripciones[rand() % descripciones.size()]};

    // Pusheamos la tarea a la cola de tareas
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        taskQueue.push(tarea);
        {
            std::lock_guard<std::mutex> consoleLock(consoleMutex);
            std::cout << "Sensor " << id << " creó tarea " << taskId << ": " << tarea.descripcionTarea << std::endl;
        }
    }

    // Avisamos a los robots que hay una tarea disponible
    taskAvailable.notify_one();
}

// Finalización del sensor
{
    std::lock_guard<std::mutex> lock(queueMutex);
    --activeSensors;
    if (activeSensors == 0) {
        allSensorsDone = true;
        {
            std::lock_guard<std::mutex> consoleLock(consoleMutex);
            std::cout << "Sensor " << id << " finalizó la generación de tareas" << std::endl;
        }
        taskAvailable.notify_all(); // Notificamos a los robots que todos los sensores terminaron
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
    // Esperamos a que haya una tarea disponible o que todos los sensores hayan terminado y que no hayan tareas pendientes
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        taskAvailable.wait(lock, [&] {
            return !taskQueue.empty() || (allSensorsDone && taskQueue.empty());
        });

        // Chequeamos si hay tareas pendientes
        if (taskQueue.empty() && allSensorsDone) {
            break;
        }

        // Obtenemos la tarea de la cola
        tarea = taskQueue.front();
        taskQueue.pop();
        taskRetrieved = true;
    }

    if (taskRetrieved) {
        // Ejecutar una tarea (250ms)
        std::this_thread::sleep_for(std::chrono::milliseconds(250));

        // Avisamos que el robot procesó la tarea
        {
            std::lock_guard<std::mutex> consoleLock(consoleMutex);
            std::cout << "Robot " << id << " procesó tarea " << tarea.idTarea << std::endl;
        }
    }
}

// Finalización del robot
{
    std::lock_guard<std::mutex> consoleLock(consoleMutex);
    std::cout << "Robot " << id << " finalizó su trabajo" << std::endl;
}

}

int main() { 
    const int numSensors = 3; 
    const int numRobots = 3; 
    const int tasksPerSensor = 10;

    // Creamos los sensores
    std::vector<std::thread> sensorThreads;
    for (int i = 0; i < numSensors; ++i) {
        sensorThreads.emplace_back(sensor, i, tasksPerSensor);
    }

    // Creamos los robots
    std::vector<std::thread> robotThreads;
    for (int i = 0; i < numRobots; ++i) {
        robotThreads.emplace_back(robot, i);
    }

    // Esperamos a que los sensores terminen
    for (auto& thread : sensorThreads) {
        thread.join();
    }

    // Esperamos a que los robots terminen
    for (auto& thread : robotThreads) {
        thread.join();
    }

    return 0;

}