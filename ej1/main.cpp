#include <iostream>
#include "headers/pokedex.hpp"

int main() {
    cout << "Iniciando programa." << endl;
    Pokedex pokedex("pokedex.csv");

    try {
        cout << "Agregando Squirtle." << endl;
        Pokemon squirtle("Squirtle", 100);
        PokemonInfo squirtleInfo(squirtle, "Agua", "Una tortuga pequeña que lanza chorros de agua.",
                                 {{"Pistola Agua", 4}, {"Hidrobomba", 6}, {"Danza Lluvia", 5}},
                                 {0, 400, 1000});
        pokedex.agregarPokemon(squirtle, squirtleInfo);

        cout << "Agregando Bulbasaur." << endl;
        Pokemon bulbasaur("Bulbasaur", 270);
        PokemonInfo bulbasaurInfo(bulbasaur, "Planta", "Tiene una semilla en su lomo que crece con el tiempo.",
                                  {{"Latigazo", 4}, {"Hoja Afilada", 6}, {"Rayo Solar", 5}},
                                  {0, 300, 1100});
        pokedex.agregarPokemon(bulbasaur, bulbasaurInfo);

        cout << "[LOG] Agregando Charmander." << endl;
        Pokemon charmander("Charmander", 633);
        PokemonInfo charmanderInfo(charmander, "Fuego", "Una lagartija con una llama en su cola.",
                                   {{"Ascuas", 4}, {"Lanzallamas", 6}, {"Giro Fuego", 5}},
                                   {0, 250, 1300});
        pokedex.agregarPokemon(charmander, charmanderInfo);

        cout << "Mostrando Squirtle (870)." << endl;
        pokedex.mostrar(Pokemon("Squirtle", 870));
        cout << "Mostrando Pikachu." << endl;
        pokedex.mostrar(Pokemon("Pikachu", 390));
        cout << "Mostrando todos." << endl;
        pokedex.mostrarTodos();
    } catch (const exception& e) {
        cerr << "Error: Excepción: " << e.what() << endl;
        return 1;
    }

    cout << "Programa terminado." << endl;
    return 0;
}