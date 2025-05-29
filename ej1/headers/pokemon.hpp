#pragma once

#include <string>

using namespace std;

class Pokemon {
private:
    string nombre;
    int experiencia;

public:
    Pokemon(string nombre, int experiencia);
    string getNombre() const;
    int getExperiencia() const;
    bool operator==(const Pokemon& other) const;
    bool operator<(const Pokemon& other) const;
    ~Pokemon() = default;
};