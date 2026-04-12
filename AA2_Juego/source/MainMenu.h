#pragma once
#include "Scene.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "RenderManager.h"
#include "Renderer.h"
#include "TextObject.h"
#include "Button.h"
#include "TextField.h"
#include <iostream>
#include <string>

class MainMenu : public Scene
{
private:
    float windowWidth = RM->WINDOW_WIDTH;
    float windowHeight = RM->WINDOW_HEIGHT;

    Object* textMenu = nullptr;
    TextField* roomField = nullptr;

    Button* btnJoin = nullptr;
    Button* btnCreate = nullptr;
    Button* buttonRanking = nullptr;
    Button* buttonAudio = nullptr;
    Button* buttonExit = nullptr;

    Object* btnJoinText = nullptr;
    Object* btnCreateText = nullptr;
    Object* buttonRankingText = nullptr;
    Object* buttonAudioText = nullptr;
    Object* buttonExitText = nullptr;

public:
    void OnEnter() override {
        //AM.PlaySong("menuMusic");

        textMenu = new TextObject("3 EN RAYA ONLINE");
        textMenu->GetTransform()->position = Vector2(windowWidth / 2 - 160.0f, windowHeight / 2 - 180);
        SPAWN.SpawnObject(textMenu);

        roomField = new TextField(Vector2(windowWidth / 2 - 125, windowHeight / 2 - 120), Vector2(250, 45));
        SPAWN.SpawnObject(roomField);

        // Horizontal layout for Join/Create buttons
        btnJoin = new Button(Vector2(windowWidth / 2 - 250, windowHeight / 2 - 60), Vector2(200, 45), sf::Color(100, 0, 100, 255), "", Button::ActionType::ChangeScene, "TicTacToe");
        btnCreate = new Button(Vector2(windowWidth / 2 + 50, windowHeight / 2 - 60), Vector2(200, 45), sf::Color(0, 100, 100, 255), "", Button::ActionType::ChangeScene, "TicTacToe");

        SPAWN.SpawnObject(btnJoin);
        SPAWN.SpawnObject(btnCreate);

        btnJoinText = new TextObject("UNIRSE");
        btnJoinText->GetTransform()->position = Vector2(windowWidth / 2 - 225.0f, windowHeight / 2 - 52);
        SPAWN.SpawnObject(btnJoinText);

        btnCreateText = new TextObject("CREAR");
        btnCreateText->GetTransform()->position = Vector2(windowWidth / 2 + 85.0f, windowHeight / 2 - 52);
        SPAWN.SpawnObject(btnCreateText);

        float btn1_Y = windowHeight / 2 + 20;
        float btn2_Y = windowHeight / 2 + 80;
        float btn3_Y = windowHeight / 2 + 160;

        float btn_X = windowWidth / 2 - 200;

        buttonRanking = new Button(Vector2(btn_X, btn1_Y), Vector2(400, 45), sf::Color(0, 100, 200, 255), "", Button::ActionType::ChangeScene, "Ranking");
        buttonAudio = new Button(Vector2(btn_X, btn2_Y), Vector2(400, 45), sf::Color(0, 100, 200, 255), "", Button::ActionType::ToggleAudio);
        buttonExit = new Button(Vector2(btn_X, btn3_Y), Vector2(400, 45), sf::Color(0, 100, 200, 255), "", Button::ActionType::ExitGame);

        SPAWN.SpawnObject(buttonRanking);
        SPAWN.SpawnObject(buttonAudio);
        SPAWN.SpawnObject(buttonExit);

        buttonRankingText = new TextObject("RANKING");
        buttonRankingText->GetTransform()->position = Vector2(windowWidth / 2 - 85.0f, btn1_Y + 7);
        SPAWN.SpawnObject(buttonRankingText);

        buttonAudioText = new TextObject("AUDIO ON/OFF");
        buttonAudioText->GetTransform()->position = Vector2(windowWidth / 2 - 140.0f, btn2_Y + 7);
        SPAWN.SpawnObject(buttonAudioText);

        buttonExitText = new TextObject("EXIT GAME");
        buttonExitText->GetTransform()->position = Vector2(windowWidth / 2 - 105.0f, btn3_Y + 7);
        SPAWN.SpawnObject(buttonExitText);
    }

    ~MainMenu()
    {
        // Manual cleanup for objects
        if (textMenu) { textMenu->Destroy(); textMenu = nullptr; }
        if (btnJoinText) { btnJoinText->Destroy(); btnJoinText = nullptr; }
        if (btnCreateText) { btnCreateText->Destroy(); btnCreateText = nullptr; }
        if (buttonRankingText) { buttonRankingText->Destroy(); buttonRankingText = nullptr; }
        if (buttonAudioText) { buttonAudioText->Destroy(); buttonAudioText = nullptr; }
        if (buttonExitText) { buttonExitText->Destroy(); buttonExitText = nullptr; }
    }

    void OnExit() override
    {
        AM.StopAudio();
        Scene::OnExit();
    }

    void Update() override
    {
        Scene::Update();

        // Sync room code field with shared data for the next scene
        if (roomField) {
            SM.sharedData = roomField->GetContent();
        }
    }

    void Render() override
    {
        Scene::Render();
    }
};