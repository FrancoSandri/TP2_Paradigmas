#include "../headers/pokemon.hpp"
#include <stdexcept>

Pokemon::Pokemon(string nombre, int experiencia) {
    if (nombre.empty()) {
        throw invalid_argument("El nombre del Pokémon no puede estar vacío.");
    }
    if (experiencia < 0) {
        throw invalid_argument("La experiencia debe ser un entero positivo.");
    }
    this->nombre = nombre;
    this->experiencia = experiencia;
}

string Pokemon::getNombre() const {
    return nombre;
}

int Pokemon::getExperiencia() const {
    return experiencia;
}

bool Pokemon::operator==(const Pokemon& other) const {
    return nombre == other.nombre;
}

bool Pokemon::operator<(const Pokemon& other) const {
    return nombre < other.nombre;
}