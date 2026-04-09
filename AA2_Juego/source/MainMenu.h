#pragma once
#include "Scene.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "RenderManager.h"
#include "Renderer.h"
#include "TextObject.h"
#include "Button.h"
#include <iostream>
#include <string>

class MainMenu : public Scene
{
private:
    float windowWidth = RM->WINDOW_WIDTH;
    float windowHeight = RM->WINDOW_HEIGHT;

    Object* textMenu = nullptr;

    Button* buttonSpaceInvaders = nullptr;
    Button* buttonAudio = nullptr;
    Button* buttonExit = nullptr;
    Button* buttonRanking = nullptr;

    Object* buttonSpaceInvadersText = nullptr;
    Object* buttonAudioText = nullptr;
    Object* buttonExitText = nullptr;
    Object* buttonRankingText = nullptr;

public:
    void OnEnter() override {
        AM.PlaySong("menuMusic");

        textMenu = new TextObject("MAIN MENU");
        textMenu->GetTransform()->position = Vector2(windowWidth / 2 - 75.0f, windowHeight / 2 - 150);
        SPAWN.SpawnObject(textMenu);

        float btn1_Y = windowHeight / 2 - 80;
        float btn2_Y = windowHeight / 2;
        float btn3_Y = windowHeight / 2 + 80;
        float btn4_Y = windowHeight / 2 + 160;

        float btn_X = windowWidth / 2 - 125.0f;

        buttonSpaceInvaders = new Button(Vector2(btn_X, btn1_Y), Vector2(250, 50), sf::Color(255, 0, 255, 255), "", Button::ActionType::ChangeScene, "TicTacToe");

        buttonRanking = new Button(Vector2(btn_X, btn2_Y), Vector2(250, 50), sf::Color(255, 0, 255, 255), "", Button::ActionType::ChangeScene, "Ranking");

        buttonAudio = new Button(Vector2(btn_X, btn3_Y), Vector2(250, 50), sf::Color(255, 0, 255, 255), "", Button::ActionType::ToggleAudio);
        buttonExit = new Button(Vector2(btn_X, btn4_Y), Vector2(250, 50), sf::Color(255, 0, 255, 255), "", Button::ActionType::ExitGame);

        SPAWN.SpawnObject(buttonSpaceInvaders);
        SPAWN.SpawnObject(buttonRanking);
        SPAWN.SpawnObject(buttonAudio);
        SPAWN.SpawnObject(buttonExit);

        buttonSpaceInvadersText = new TextObject("TIC TAC TOE");
        buttonSpaceInvadersText->GetTransform()->position = Vector2(windowWidth / 2 - 70.0f, btn1_Y + 10);
        SPAWN.SpawnObject(buttonSpaceInvadersText);

        buttonRankingText = new TextObject("RANKING");
        buttonRankingText->GetTransform()->position = Vector2(windowWidth / 2 - 50.0f, btn2_Y + 10);
        SPAWN.SpawnObject(buttonRankingText);

        buttonAudioText = new TextObject("TOGGLE AUDIO");
        buttonAudioText->GetTransform()->position = Vector2(windowWidth / 2 - 80.0f, btn3_Y + 10);
        SPAWN.SpawnObject(buttonAudioText);

        buttonExitText = new TextObject("EXIT GAME");
        buttonExitText->GetTransform()->position = Vector2(windowWidth / 2 - 60.0f, btn4_Y + 10);
        SPAWN.SpawnObject(buttonExitText);
    }

    ~MainMenu()
    {
        if (buttonSpaceInvadersText) { buttonSpaceInvadersText->Destroy(); buttonSpaceInvadersText = nullptr; }
        if (buttonAudioText) { buttonAudioText->Destroy(); buttonAudioText = nullptr; }
        if (buttonExitText) { buttonExitText->Destroy(); buttonExitText = nullptr; }
        if (textMenu) { textMenu->Destroy(); textMenu = nullptr; }
    }

    void OnExit() override
    {
        AM.StopAudio();
        Scene::OnExit();
    }

    void Update() override
    {
        Scene::Update();
    }

    void Render() override
    {
        Scene::Render();
    }
};