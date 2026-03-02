#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include "Scene.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "RenderManager.h"
#include "Renderer.h"
#include "TextObject.h"
#include "Button.h"

class RankingMenu : public Scene {
private:
    float windowWidth = RM->WINDOW_WIDTH;
    float windowHeight = RM->WINDOW_HEIGHT;

    std::vector<std::pair<std::string, int>> spaceInvadersScores;
    std::vector<std::pair<std::string, int>> tanksScores;
    std::vector<std::pair<std::string, int>> splatScores;

    Object* textRanking;
    Button* buttonBack;
    Object* buttonBacktext;

public:
    void OnEnter() override {
        AM.PlaySong("rankingMenu");
        textRanking = new TextObject("RANKING");
        textRanking->GetTransform()->position = Vector2(windowWidth / 2 - 30, 75.0f);
        SPAWN.SpawnObject(textRanking);

        buttonBack = new Button(Vector2(50, windowHeight - 100), Vector2(250, 50), SDL_Color{ 255, 0, 255, 255 }, "Main Menu", Button::ActionType::ChangeScene, "MainMenu");
        SPAWN.SpawnObject(buttonBack);

        buttonBacktext = new TextObject("BACK");
        buttonBacktext->GetTransform()->position = Vector2(195, windowHeight - 40);
        SPAWN.SpawnObject(buttonBacktext);

        LoadRankingFromFile("resources/highScore.bin");
        DisplayScores();
    }

    ~RankingMenu() {
        textRanking->Destroy();
        textRanking = nullptr;

        buttonBacktext->Destroy();
        buttonBacktext = nullptr;
    }

    void OnExit() override {
        AM.StopAudio();
        Scene::OnExit();
    }

    void Update() override {
        Scene::Update();
    }

    void Render() override {
        Scene::Render();
    }

    void LoadRankingFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) {
            std::cerr << "Error: No se pudo abrir el archivo " << filename << std::endl;
            return;
        }

        std::string line;
        int counter = 0;

        while (std::getline(file, line)) {
            if (line.empty()) continue; 

            // Extraer nombre y puntaje
            std::string name = line.substr(0, 5); 
            int score = std::stoi(line.substr(6)); 

            // Asignar los puntajes a cada categoría (10 para cada una)
            if (counter < 10) {
                spaceInvadersScores.emplace_back(name, score);
            }
            else if (counter < 20) {
                tanksScores.emplace_back(name, score);
            }
            else if (counter < 30) {
                splatScores.emplace_back(name, score);
            }

            counter++;
        }

        file.close();
    }


    // Mostrar puntajes en pantalla
    void DisplayScores() {
        float baseY = 150.0f;
        float columnSpacing = 400.0f; 
        float startX = 250.0f;

        // Títulos
        Object* spaceTitle = new TextObject("Space Invaders:");
        spaceTitle->GetTransform()->position = Vector2(startX, baseY);
        SPAWN.SpawnObject(spaceTitle);

        Object* tanksTitle = new TextObject("Tanks:");
        tanksTitle->GetTransform()->position = Vector2(startX + columnSpacing, baseY);
        SPAWN.SpawnObject(tanksTitle);

        Object* splatTitle = new TextObject("Splat!:");
        splatTitle->GetTransform()->position = Vector2(startX + 2 * columnSpacing, baseY);
        SPAWN.SpawnObject(splatTitle);

        // Mostrar puntajes en columnas

        for (size_t i = 0; i < 10; ++i) {
            std::string spaceText = std::to_string(i + 1) + ". " + spaceInvadersScores[i].first + " " + std::to_string(spaceInvadersScores[i].second);
            std::string tanksText = std::to_string(i + 1) + ". " + tanksScores[i].first + " " + std::to_string(tanksScores[i].second);
            std::string splatText = std::to_string(i + 1) + ". " + splatScores[i].first + " " + std::to_string(splatScores[i].second);

            Object* spaceScore = new TextObject(spaceText);
            spaceScore->GetTransform()->position = Vector2(startX, baseY + 30.0f * (i + 1));
            SPAWN.SpawnObject(spaceScore);

            Object* tanksScore = new TextObject(tanksText);
            tanksScore->GetTransform()->position = Vector2(startX + columnSpacing, baseY + 30.0f * (i + 1));
            SPAWN.SpawnObject(tanksScore);

            Object* splatScore = new TextObject(splatText);
            splatScore->GetTransform()->position = Vector2(startX + 2 * columnSpacing, baseY + 30.0f * (i + 1));
            SPAWN.SpawnObject(splatScore);
        }
    }
};
