#include "../headers/pokemoninfo.hpp"
#include <stdexcept>
#include <unordered_set>

bool PokemonInfo::esTipoValido(const string& tipo) {
    static const unordered_set<string> tiposValidos = {
        "Agua", "Fuego", "Planta", "Eléctrico", "Hielo", "Lucha", "Veneno", "Tierra"
    };
    return tiposValidos.find(tipo) != tiposValidos.end();
}

PokemonInfo::PokemonInfo(Pokemon pokemon, string tipo, string descripcion,
                         unordered_map<string, int> ataquesDisponiblesPorNivel,
                         array<int, 3> experienciaProximoNivel)
    : pokemon(pokemon), tipo(tipo), descripcion(descripcion),
      ataquesDisponiblesPorNivel(ataquesDisponiblesPorNivel),
      experienciaProximoNivel(experienciaProximoNivel) {
    if (!esTipoValido(tipo)) {
        throw invalid_argument("Tipo de Pokémon inválido.");
    }
    if (descripcion.empty()) {
        throw invalid_argument("La descripción no puede estar vacía.");
    }
    if (ataquesDisponiblesPorNivel.empty()) {
        throw invalid_argument("Debe haber al menos un ataque disponible.");
    }
    for (const auto& ataque : ataquesDisponiblesPorNivel) {
        if (ataque.second <= 0) {
            throw invalid_argument("El daño de los ataques debe ser positivo.");
        }
    }
    for (int exp : experienciaProximoNivel) {
        if (exp < 0) {
            throw invalid_argument("La experiencia para el próximo nivel debe ser positiva.");
        }
    }
}

Pokemon PokemonInfo::getPokemon() const {
    return pokemon;
}

const string PokemonInfo::getTipo() const {
    return tipo;
}

const string PokemonInfo::getDescripcion() const {
    return descripcion;
}

const unordered_map<string, int> PokemonInfo::getAtaquesDisponiblesPorNivel() const {
    return ataquesDisponiblesPorNivel;
}

const array<int, 3> PokemonInfo::getExperienciaProximoNivel() const {
    return experienciaProximoNivel;
}