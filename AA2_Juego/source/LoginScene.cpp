#include "LoginScene.h"
#include "SceneManager.h"
#include "RenderManager.h"
#include "Spawner.h"
#include "AudioManager.h"

LoginScene::LoginScene() : Scene() {
    winW = RM->WINDOW_WIDTH;
    winH = RM->WINDOW_HEIGHT;

    titleText = nullptr;
    userField = nullptr;
    passField = nullptr;
    btnLogin = nullptr;
    btnRegister = nullptr;
    userLabel = nullptr;
    passLabel = nullptr;
    btnLoginText = nullptr;
    btnRegisterText = nullptr;
}

void LoginScene::OnEnter() {
    AM.SetMusicVolume(10.0f);
    AM.PlaySong("menuMusic");

    
    titleText = new TextObject("3 EN RAYA ONLINE");
    titleText->GetTransform()->position = Vector2(winW / 2 - 120.0f, winH / 2 - 200);
    titleText->SetColor(sf::Color::Yellow);
    SPAWN.SpawnObject(titleText);

    userField = new TextField(Vector2(winW / 2 - 125, winH / 2 - 100), Vector2(250, 45));
    passField = new TextField(Vector2(winW / 2 - 125, winH / 2 - 20), Vector2(250, 45));
    SPAWN.SpawnObject(userField);
    SPAWN.SpawnObject(passField);

    userLabel = new TextObject("NICKNAME:");
    userLabel->GetTransform()->position = Vector2(winW / 2 - 125, winH / 2 - 130);
    SPAWN.SpawnObject(userLabel);

    passLabel = new TextObject("PASSWORD:");
    passLabel->GetTransform()->position = Vector2(winW / 2 - 125, winH / 2 - 50);
    SPAWN.SpawnObject(passLabel);

    btnLogin = new Button(Vector2(winW / 2 - 125, winH / 2 + 60), Vector2(250, 45), sf::Color(0, 100, 200, 255), "", Button::ActionType::ChangeScene, "MainMenu");
    btnRegister = new Button(Vector2(winW / 2 - 125, winH / 2 + 120), Vector2(250, 45), sf::Color(0, 150, 0, 255), "", Button::ActionType::ChangeScene, "MainMenu");

    SPAWN.SpawnObject(btnLogin);
    SPAWN.SpawnObject(btnRegister);

    btnLoginText = new TextObject("LOGIN");
    btnLoginText->GetTransform()->position = Vector2(winW / 2 - 40.0f, winH / 2 + 70);
    SPAWN.SpawnObject(btnLoginText);

    btnRegisterText = new TextObject("REGISTRO");
    btnRegisterText->GetTransform()->position = Vector2(winW / 2 - 60.0f, winH / 2 + 130);
    SPAWN.SpawnObject(btnRegisterText);
}

void LoginScene::OnExit() {
    Scene::OnExit();
}

void LoginScene::Update() {
    Scene::Update();
}

void LoginScene::Render() {
    Scene::Render();
}