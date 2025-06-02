#include "../headers/pokedex.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

Pokedex::Pokedex() : archivo("") {
    std::cout << "Pokedex creada sin archivo." << std::endl;
}

Pokedex::Pokedex(const std::string& archivo) : archivo(archivo) {
    std::cout << "Pokedex creada con archivo: " << archivo << std::endl;
    cargarDesdeArchivo();
}

void Pokedex::cargarDesdeArchivo() {
    if (archivo.empty()) {
        std::cout << "Error: No se especificó archivo para cargar." << std::endl;
        return;
    }
    std::ifstream file(archivo, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo: " << archivo << std::endl;
        return;
    }

    while (file.peek() != EOF) {
        try {
            // Read string lengths and strings
            uint32_t nombreLen, tipoLen, descLen;
            file.read(reinterpret_cast<char*>(&nombreLen), sizeof(nombreLen));
            if (file.eof()) break; // End of file reached
            std::string nombre(nombreLen, '\0');
            file.read(&nombre[0], nombreLen);

            int experiencia;
            file.read(reinterpret_cast<char*>(&experiencia), sizeof(experiencia));

            file.read(reinterpret_cast<char*>(&tipoLen), sizeof(tipoLen));
            std::string tipo(tipoLen, '\0');
            file.read(&tipo[0], tipoLen);

            file.read(reinterpret_cast<char*>(&descLen), sizeof(descLen));
            std::string descripcion(descLen, '\0');
            file.read(&descripcion[0], descLen);

            // Read attacks
            std::unordered_map<std::string, int> ataques;
            for (int i = 0; i < 3; ++i) {
                uint32_t ataqueNombreLen;
                file.read(reinterpret_cast<char*>(&ataqueNombreLen), sizeof(ataqueNombreLen));
                std::string ataqueNombre(ataqueNombreLen, '\0');
                file.read(&ataqueNombre[0], ataqueNombreLen);
                int ataqueDanio;
                file.read(reinterpret_cast<char*>(&ataqueDanio), sizeof(ataqueDanio));
                ataques[ataqueNombre] = ataqueDanio;
            }

            // Read experience levels
            std::array<int, 3> expProximoNivel;
            for (int i = 0; i < 3; ++i) {
                file.read(reinterpret_cast<char*>(&expProximoNivel[i]), sizeof(expProximoNivel[i]));
            }

            Pokemon pokemon(nombre, experiencia);
            PokemonInfo info(pokemon, tipo, descripcion, ataques, expProximoNivel);
            pokedex.insert({pokemon, info});
            std::cout << "Cargado Pokémon: " << nombre << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error al leer Pokémon: " << e.what() << std::endl;
            break;
        }
    }
    file.close();
    std::cout << "Carga de archivo completada." << std::endl;
}

void Pokedex::guardarEnArchivo() const {
    if (archivo.empty()) {
        std::cout << "Error: No se especificó archivo para guardar." << std::endl;
        return;
    }
    std::ofstream file(archivo, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Error: No se pudo abrir el archivo para escritura: " + archivo);
    }

    std::cout << "Guardando " << pokedex.size() << " Pokémon en " << archivo << std::endl;
    for (const auto& entry : pokedex) {
        const Pokemon& pokemon = entry.first;
        const PokemonInfo& info = entry.second;

        // Write nombre
        const std::string& nombre = pokemon.getNombre();
        uint32_t nombreLen = static_cast<uint32_t>(nombre.size());
        file.write(reinterpret_cast<const char*>(&nombreLen), sizeof(nombreLen));
        file.write(nombre.data(), nombreLen);

        // Write experiencia
        int experiencia = pokemon.getExperiencia();
        file.write(reinterpret_cast<const char*>(&experiencia), sizeof(experiencia));

        // Write tipo
        const std::string& tipo = info.getTipo();
        uint32_t tipoLen = static_cast<uint32_t>(tipo.size());
        file.write(reinterpret_cast<const char*>(&tipoLen), sizeof(tipoLen));
        file.write(tipo.data(), tipoLen);

        // Write descripcion
        const std::string& descripcion = info.getDescripcion();
        uint32_t descLen = static_cast<uint32_t>(descripcion.size());
        file.write(reinterpret_cast<const char*>(&descLen), sizeof(descLen));
        file.write(descripcion.data(), descLen);

        // Write attacks
        for (const auto& ataque : info.getAtaquesDisponiblesPorNivel()) {
            uint32_t ataqueNombreLen = static_cast<uint32_t>(ataque.first.size());
            file.write(reinterpret_cast<const char*>(&ataqueNombreLen), sizeof(ataqueNombreLen));
            file.write(ataque.first.data(), ataqueNombreLen);
            int ataqueDanio = ataque.second;
            file.write(reinterpret_cast<const char*>(&ataqueDanio), sizeof(ataqueDanio));
        }

        // Write experience levels
        for (int exp : info.getExperienciaProximoNivel()) {
            file.write(reinterpret_cast<const char*>(&exp), sizeof(exp));
        }
    }
    file.close();
    std::cout << "Guardado completado." << std::endl;
}

void Pokedex::agregarPokemon(const Pokemon& pokemon, const PokemonInfo& info) {
    if (pokedex.find(pokemon) != pokedex.end()) {
        throw std::invalid_argument(pokemon.getNombre() + " ya existe en la Pokedex.");
    }
    pokedex.insert({pokemon, info});
    std::cout << "Agregado Pokémon: " << pokemon.getNombre() << std::endl;
    guardarEnArchivo();
    std::cout << pokemon.getNombre() << " fue agregado a la Pokedex." << std::endl;
}

void Pokedex::mostrar(const Pokemon& pokemon) const {
    auto it = pokedex.find(pokemon);
    if (it != pokedex.end()) {
        const PokemonInfo& info = it->second;
        std::cout << "Nombre: " << info.getPokemon().getNombre() << "\n"
                  << "Experiencia: " << info.getPokemon().getExperiencia() << "\n"
                  << "Tipo: " << info.getTipo() << "\n"
                  << "Descripción: " << info.getDescripcion() << "\n"
                  << "Ataques Disponibles:\n";
        for (const auto& ataque : info.getAtaquesDisponiblesPorNivel()) {
            std::cout << "  - " << ataque.first << " (Daño: " << ataque.second << ")\n";
        }
        std::cout << "Experiencia para Próximos Niveles:" << std::endl;
        for (size_t i = 0; i < info.getExperienciaProximoNivel().size(); ++i) {
            std::cout << "  - Nivel " << i << ": " << info.getExperienciaProximoNivel()[i] << "\n";
        }
    } else {
        std::cout << "¡Pokémon desconocido!" << std::endl;
    }
}

void Pokedex::mostrarTodos() const {
    if (pokedex.empty()) {
        std::cout << "La Pokedex está vacía." << std::endl;
        return;
    }
    std::cout << "Mostrando todos los Pokémon." << std::endl;
    for (const auto& entry : pokedex) {
        mostrar(entry.first);
        std::cout << "------------------------\n";
    }
}

Pokedex::~Pokedex() {
    std::cout << "Destruyendo Pokedex." << std::endl;
    if (!archivo.empty()) {
        std::cout << "Pokedex destruida." << std::endl;
    }
}