#pragma once

#include "pokemon.hpp"
#include <unordered_map>
#include <array>
#include <string>

class PokemonInfo {
private:
    Pokemon pokemon;
    string tipo;
    string descripcion;
    unordered_map<string, int> ataquesDisponiblesPorNivel;
    array<int, 3> experienciaProximoNivel;

    static bool esTipoValido(const std::string& tipo);

public:
    PokemonInfo(Pokemon pokemon, string tipo, std::string descripcion,
                unordered_map<string, int> ataquesDisponiblesPorNivel,
                array<int, 3> experienciaProximoNivel);
    Pokemon getPokemon() const;
    const string getTipo() const;
    const string getDescripcion() const;
    const unordered_map<string, int> getAtaquesDisponiblesPorNivel() const;
    const array<int, 3> getExperienciaProximoNivel() const;
    ~PokemonInfo() = default;
};