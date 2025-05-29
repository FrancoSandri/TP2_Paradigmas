#include "../headers/pokedex.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

Pokedex::Pokedex() : archivo("") {
    cout << "Pokedex creada sin archivo." << endl;
}

Pokedex::Pokedex(const string& archivo) : archivo(archivo) {
    cout << "Pokedex creada con archivo: " << archivo << endl;
    cargarDesdeArchivo();
}

void Pokedex::cargarDesdeArchivo() {
    if (archivo.empty()) {
        cout << "Error: No se especificó archivo para cargar." << endl;
        return;
    }
    ifstream file(archivo);
    if (!file.is_open()) {
        cerr << "Error: No se pudo abrir el archivo: " << archivo << endl;
        return;
    }

    string linea;
    int lineaNum = 0;
    while (getline(file, linea)) {
        lineaNum++;
        if (linea.empty()) {
            cout << "Línea " << lineaNum << " vacía, omitiendo." << endl;
            continue;
        }

        stringstream ss(linea);
        string nombre, tipo, descripcion, ataqueNombre, token;
        int experiencia, ataqueDanio;
        unordered_map<string, int> ataques;
        array<int, 3> expProximoNivel;

        try {
            // Parse nombre (quoted)
            if (ss.peek() != '"') throw runtime_error("Falta comilla inicial en nombre.");
            ss.ignore(1);
            getline(ss, nombre, '"');
            ss.ignore(1); // Skip comma

            // Parse experiencia
            ss >> experiencia;
            if (ss.fail()) throw runtime_error("Experiencia inválida.");
            ss.ignore(1);

            // Parse tipo (quoted)
            if (ss.peek() != '"') throw runtime_error("Falta comilla inicial en tipo.");
            ss.ignore(1);
            getline(ss, tipo, '"');
            ss.ignore(1);

            // Parse descripcion (quoted)
            if (ss.peek() != '"') throw runtime_error("Falta comilla inicial en descripción.");
            ss.ignore(1);
            getline(ss, descripcion, '"');
            ss.ignore(1);

            // Parse ataques (3 ataques)
            for (int i = 0; i < 3; ++i) {
                if (ss.peek() != '"') throw runtime_error("Falta comilla inicial en ataque.");
                ss.ignore(1);
                getline(ss, ataqueNombre, '"');
                ss.ignore(1); // Skip colon
                ss >> ataqueDanio;
                if (ss.fail()) throw runtime_error("Daño de ataque inválido.");
                ss.ignore(1); // Skip comma
                ataques[ataqueNombre] = ataqueDanio;
            }

            // Parse experienciaProximoNivel
            for (int i = 0; i < 3; ++i) {
                ss >> expProximoNivel[i];
                if (ss.fail()) throw runtime_error("Experiencia de nivel inválida.");
                if (i < 2) ss.ignore(1);
            }

            Pokemon pokemon(nombre, experiencia);
            PokemonInfo info(pokemon, tipo, descripcion, ataques, expProximoNivel);
            pokedex.insert({pokemon, info});
            cout << "Cargado Pokémon: " << nombre << endl;
        } catch (const exception& e) {
            cerr << "Error: Línea " << lineaNum << " inválida: " << e.what() << endl;
            continue;
        }
    }
    file.close();
    cout << "Carga de archivo completada." << endl;
}

void Pokedex::guardarEnArchivo() const {
    if (archivo.empty()) {
        cout << "Error: No se especificó archivo para guardar." << endl;
        return;
    }
    ofstream file(archivo);
    if (!file.is_open()) {
        throw runtime_error("Error: No se pudo abrir el archivo para escritura: " + archivo);
    }

    cout << "Guardando " << pokedex.size() << " Pokémon en " << archivo << endl;
    for (const auto& entry : pokedex) {
        const Pokemon& pokemon = entry.first;
        const PokemonInfo& info = entry.second;
        file << "\"" << pokemon.getNombre() << "\"," << pokemon.getExperiencia() << ","
             << "\"" << info.getTipo() << "\",\"" << info.getDescripcion() << "\",";
        
        int ataqueCount = 0;
        for (const auto& ataque : info.getAtaquesDisponiblesPorNivel()) {
            file << "\"" << ataque.first << "\":" << ataque.second;
            if (++ataqueCount < 3) file << ",";
        }
        file << ",";
        for (size_t i = 0; i < info.getExperienciaProximoNivel().size(); ++i) {
            file << info.getExperienciaProximoNivel()[i];
            if (i < 2) file << ",";
        }
        file << "\n";
    }
    file.close();
    cout << "Guardado completado." << endl;
}

void Pokedex::agregarPokemon(const Pokemon& pokemon, const PokemonInfo& info) {
    if (pokedex.find(pokemon) != pokedex.end()) {
        throw invalid_argument(pokemon.getNombre() + " ya existe en la Pokedex.");
    }
    pokedex.insert({pokemon, info});
    cout << "Agregado Pokémon: " << pokemon.getNombre() << endl;
    guardarEnArchivo();
    cout << pokemon.getNombre() << " fue agregado a la Pokedex." << endl;
}

void Pokedex::mostrar(const Pokemon& pokemon) const {
    auto it = pokedex.find(pokemon);
    if (it != pokedex.end()) {
        const PokemonInfo& info = it->second;
        cout << "Nombre: " << info.getPokemon().getNombre() << "\n"
                  << "Experiencia: " << info.getPokemon().getExperiencia() << "\n"
                  << "Tipo: " << info.getTipo() << "\n"
                  << "Descripción: " << info.getDescripcion() << "\n"
                  << "Ataques Disponibles:\n";
        for (const auto& ataque : info.getAtaquesDisponiblesPorNivel()) {
            cout << "  - " << ataque.first << " (Daño: " << ataque.second << ")\n";
        }
        cout << "Experiencia para Próximos Niveles:" << endl;
        for (size_t i = 0; i < info.getExperienciaProximoNivel().size(); ++i) {
            cout << "  - Nivel " << i << ": " << info.getExperienciaProximoNivel()[i] << "\n";
        }
    } else {
        cout << "¡Pokémon desconocido!" << endl;
    }
}

void Pokedex::mostrarTodos() const {
    if (pokedex.empty()) {
        cout << "La Pokedex está vacía." << endl;
        return;
    }
    cout << "Mostrando todos los Pokémon." << endl;
    for (const auto& entry : pokedex) {
        mostrar(entry.first);
        cout << "------------------------\n";
    }
}

Pokedex::~Pokedex() {
    cout << "Destruyendo Pokedex." << endl;
    if (!archivo.empty()) {
        cout << "Pokedex destruida." << endl;
    }
}