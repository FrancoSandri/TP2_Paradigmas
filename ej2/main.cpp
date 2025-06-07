#include "../headers/dron.hpp" 
#include <vector>
#include <array>
#include <iostream>

int main() { // Create five mutexes for the interference zones 
    
    std::array<std::mutex, 5> zones;

    // Create five drones, each with its adjacent zones
    std::vector<std::unique_ptr<Dron>> drones;
    for (int i = 0; i < 5; ++i) {
        // Left zone is i-1 (or 4 if i=0), right zone is i
        int leftZoneIdx = (i == 0) ? 4 : i - 1;
        drones.emplace_back(std::make_unique<Dron>(i, zones[leftZoneIdx], zones[i]));
    }

    // Start all drones
    for (auto& drone : drones) {
        drone->start();
    }

    // Wait for all drones to complete
    for (auto& drone : drones) {
        drone->join();
    }

    return 0;

}