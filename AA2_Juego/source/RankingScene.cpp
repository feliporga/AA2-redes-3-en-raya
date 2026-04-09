#include "RankingScene.h"
#include "SceneManager.h"
#include "RenderManager.h"
#include "Spawner.h"

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
    TextObject* tName = new TextObject(name);
    tName->GetTransform()->position = Vector2(300.0f, yOffset);
    SPAWN.SpawnObject(tName);
    tableTexts.push_back(tName);

    TextObject* tWins = new TextObject(wins);
    tWins->GetTransform()->position = Vector2(600.0f, yOffset);
    SPAWN.SpawnObject(tWins);
    tableTexts.push_back(tWins);

    TextObject* tLosses = new TextObject(losses);
    tLosses->GetTransform()->position = Vector2(800.0f, yOffset);
    SPAWN.SpawnObject(tLosses);
    tableTexts.push_back(tLosses);

    TextObject* tElo = new TextObject(elo);
    tElo->GetTransform()->position = Vector2(1000.0f, yOffset);
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
    headerName->GetTransform()->position = Vector2(300.0f, headerY);
    headerName->SetColor(sf::Color(200, 200, 200, 255));
    SPAWN.SpawnObject(headerName);

    headerWins = new TextObject("VICTORIAS");
    headerWins->GetTransform()->position = Vector2(600.0f, headerY);
    headerWins->SetColor(sf::Color(200, 200, 200, 255));
    SPAWN.SpawnObject(headerWins);

    headerLosses = new TextObject("DERROTAS");
    headerLosses->GetTransform()->position = Vector2(800.0f, headerY);
    headerLosses->SetColor(sf::Color(200, 200, 200, 255));
    SPAWN.SpawnObject(headerLosses);

    headerElo = new TextObject("ELO");
    headerElo->GetTransform()->position = Vector2(1000.0f, headerY);
    headerElo->SetColor(sf::Color(200, 200, 200, 255));
    SPAWN.SpawnObject(headerElo);

    CreateDummyRow(220.0f, "1. Felipe", "42", "5", "1500");
    CreateDummyRow(270.0f, "2. ProGamer", "38", "12", "1420");
    CreateDummyRow(320.0f, "3. Alex", "25", "20", "1250");
    CreateDummyRow(370.0f, "4. NoobMaster", "2", "50", "400");

    backButton = new Button(Vector2(50, 50), Vector2(150, 50), sf::Color(255, 0, 255, 255), "", Button::ActionType::ChangeScene, "MainMenu");
    SPAWN.SpawnObject(backButton);

    backButtonText = new TextObject("VOLVER");
    backButtonText->GetTransform()->position = Vector2(75.0f, 60.0f);
    SPAWN.SpawnObject(backButtonText);
}

void RankingScene::OnExit() {
    Scene::OnExit();
    tableTexts.clear();
}

void RankingScene::Update() {
    Scene::Update();
}

void RankingScene::Render() {
    Scene::Render();

    RM->DrawRect(280, 190, 800, 2, 255, 255, 255, 255);
}