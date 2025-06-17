#include "../headers/dron.hpp" 
#include <vector>
#include <array>
#include <iostream>

int main() { // Creamos 5 mutex para representar las zonas
    
    std::array<std::mutex, 5> zones;

    // Creamos 5 drones, cada uno con su zona izquierda y derecha
    std::vector<std::unique_ptr<Dron>> drones;
    for (int i = 0; i < 5; ++i) {
        // El indice de la zona izquierda es i-1, y el de la derecha es i.
        int leftZoneIdx = (i == 0) ? 4 : i - 1;
        drones.emplace_back(std::make_unique<Dron>(i, zones[leftZoneIdx], zones[i]));
    }

    // Iniciamos todos los drones
    for (auto& drone : drones) {
        drone->start();
    }

    // Esperamos a que todos los drones terminen su ejecución
    // Esto asegura que todos los drones hayan completado su tarea antes de finalizar el programa.
    for (auto& drone : drones) {
        drone->join();
    }

    return 0;

}