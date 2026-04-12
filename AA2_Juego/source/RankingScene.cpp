#include "RankingScene.h"
#include "SceneManager.h"
#include "RenderManager.h"
#include "Spawner.h"
#include "NetworkManager.h"

RankingScene::RankingScene() : Scene() {
    titleText = nullptr;
    headerName = nullptr;
    headerWins = nullptr;
    headerLosses = nullptr;
    headerElo = nullptr;
    backButton = nullptr;
    backButtonText = nullptr;
}

void RankingScene::CreateDummyRow(float yOffset, std::string name, std::string wins, std::string losses, std::string elo) {
    float winW = RM->WINDOW_WIDTH;

    TextObject* tName = new TextObject(name);
    tName->GetTransform()->position = Vector2(winW * 0.15f, yOffset);
    tName->SetColor(sf::Color::White);
    SPAWN.SpawnObject(tName);
    tableTexts.push_back(tName);

    TextObject* tWins = new TextObject(wins);
    tWins->GetTransform()->position = Vector2(winW * 0.45f, yOffset);
    tWins->SetColor(sf::Color::White);
    SPAWN.SpawnObject(tWins);
    tableTexts.push_back(tWins);

    TextObject* tLosses = new TextObject(losses);
    tLosses->GetTransform()->position = Vector2(winW * 0.65f, yOffset);
    tLosses->SetColor(sf::Color::White);
    SPAWN.SpawnObject(tLosses);
    tableTexts.push_back(tLosses);

    TextObject* tElo = new TextObject(elo);
    tElo->GetTransform()->position = Vector2(winW * 0.85f, yOffset);
    tElo->SetColor(sf::Color::White);
    SPAWN.SpawnObject(tElo);
    tableTexts.push_back(tElo);
}

void RankingScene::OnEnter() {
    float winW = RM->WINDOW_WIDTH;

    titleText = new TextObject("RANKING DE JUGADORES");
    titleText->GetTransform()->position = Vector2(winW / 2 - 150.0f, 50.0f);
    titleText->SetColor(sf::Color::Yellow);
    SPAWN.SpawnObject(titleText);

    float headerY = 150.0f;

    headerName = new TextObject("NOMBRE");
    headerName->GetTransform()->position = Vector2(winW * 0.15f, headerY);
    headerName->SetColor(sf::Color(200, 200, 200, 255));
    SPAWN.SpawnObject(headerName);

    headerWins = new TextObject("VICTORIAS");
    headerWins->GetTransform()->position = Vector2(winW * 0.45f, headerY);
    headerWins->SetColor(sf::Color(200, 200, 200, 255));
    SPAWN.SpawnObject(headerWins);

    headerLosses = new TextObject("DERROTAS");
    headerLosses->GetTransform()->position = Vector2(winW * 0.65f, headerY);
    headerLosses->SetColor(sf::Color(200, 200, 200, 255));
    SPAWN.SpawnObject(headerLosses);

    headerElo = new TextObject("ELO");
    headerElo->GetTransform()->position = Vector2(winW * 0.85f, headerY);
    headerElo->SetColor(sf::Color(200, 200, 200, 255));
    SPAWN.SpawnObject(headerElo);

    backButton = new Button(Vector2(50, 50), Vector2(210, 50), sf::Color(0, 100, 200, 255), "", Button::ActionType::ChangeScene, "MainMenu");
    SPAWN.SpawnObject(backButton);

    backButtonText = new TextObject("VOLVER");
    backButtonText->GetTransform()->position = Vector2(75.0f, 60.0f);
    SPAWN.SpawnObject(backButtonText);

    NM.newRankingAvailable = false;
    NM.RequestRanking();
}

void RankingScene::OnExit() {
    Scene::OnExit();
    tableTexts.clear();
}

void RankingScene::Update() {
    Scene::Update();

    if (NM.newRankingAvailable) {
        float startY = 220.0f;

        for (const auto& player : NM.lastRanking) {
            std::string displayName = std::to_string(player.pos) + ". " + player.name;

            if (player.pos > 10) {
                startY += 20.0f;
            }

            CreateDummyRow(startY, displayName, std::to_string(player.v), std::to_string(player.d), std::to_string(player.pts));
            startY += 50.0f;
        }

        NM.newRankingAvailable = false;
    }
}

void RankingScene::Render() {
    Scene::Render();

    float winW = RM->WINDOW_WIDTH;
    RM->DrawRect((int)(winW * 0.15f), 190, (int)(winW * 0.80f), 2, 255, 255, 255, 255);
}