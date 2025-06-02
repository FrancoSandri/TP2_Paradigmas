#pragma once

#include "pokemon.hpp"
#include "pokemoninfo.hpp"
#include <unordered_map>
#include <string>

class PokemonHash {
    public:
        size_t operator()(const Pokemon& p) const {
            return hash<string>()(p.getNombre());
        }
};

class Pokedex {
private:
    unordered_map<Pokemon, PokemonInfo, PokemonHash> pokedex;
    string archivo;

    void cargarDesdeArchivo();
    void guardarEnArchivo() const;

public:
    Pokedex();
    Pokedex(const string& archivo);
    void agregarPokemon(const Pokemon& pokemon, const PokemonInfo& info);
    void mostrar(const Pokemon& pokemon) const;
    void mostrarTodos() const;
    ~Pokedex();
};