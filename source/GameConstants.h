#ifndef GAME_CONSTANTS_H
#define GAME_CONSTANTS_H

#include "../dependencies/rapidXML/rapidxml.hpp"
#include "../dependencies/rapidXML/rapidxml_utils.hpp"
#include "../dependencies/rapidXML/rapidxml_iterators.hpp"
#include "../dependencies/rapidXML/rapidxml_print.hpp"
#include <unordered_map>
#include <string>
#include <iostream>
#include <stdexcept>

// Estructura para almacenar las constantes de un juego
struct GameConstants {
    std::unordered_map<std::string, float> constants;

    // Obtener el valor de una constante
    float GetValue(const std::string& name, float defaultValue = 0.0f) const {
        auto it = constants.find(name);
        if (it != constants.end()) {
            return it->second;
        }
        return defaultValue;
    }
};

// Clase para manejar las constantes de todos los juegos
class GameConstantsManager {
private:
    std::unordered_map<std::string, GameConstants> constantsByGame;

public:
    // Cargar las constantes desde un archivo XML
    void LoadFromXML(const std::string& filename) {
        try {
            rapidxml::file<> xmlFile(filename.c_str());
            rapidxml::xml_document<> doc;
            doc.parse<0>(xmlFile.data());

            auto* root = doc.first_node("ConstantsByGame");
            if (!root) throw std::runtime_error("Nodo raíz 'ConstantsByGame' no encontrado.");

            for (auto* gameNode = root->first_node(); gameNode; gameNode = gameNode->next_sibling()) {
                std::string gameName = gameNode->name();
                GameConstants gameConstants;

                for (auto* constantNode = gameNode->first_node("Constant"); constantNode; constantNode = constantNode->next_sibling()) {
                    std::string name = constantNode->first_attribute("name")->value();
                    float value = std::stof(constantNode->first_attribute("value")->value());
                    gameConstants.constants[name] = value;
                }

                constantsByGame[gameName] = gameConstants;
            }

            std::cout << "Constantes cargadas correctamente desde " << filename << std::endl;
        }
        catch (const std::exception& e) {
            std::cerr << "Error al cargar el archivo XML: " << e.what() << std::endl;
        }
    }

    // Obtener las constantes de un juego específico
    const GameConstants& GetGameConstants(const std::string& gameName) const {
        auto it = constantsByGame.find(gameName);
        if (it != constantsByGame.end()) {
            return it->second;
        }
        throw std::runtime_error("No se encontraron constantes para el juego: " + gameName);
    }
};

#endif // GAME_CONSTANTS_H
