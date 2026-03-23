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

    Object* textMenu;

    Button* buttonSplat;
    Button* buttonSpaceInvaders;
    Button* buttonTanks;
    Button* buttonSprite;
    Button* buttonRanking;
    Button* buttonAudio;
    Button* buttonExit;

    Object* buttonSplatText;
    Object* buttonSpaceInvadersText;
    Object* buttonTanksText;
    Object* buttonSpriteText;
    Object* buttonRankingText;
    Object* buttonAudioText;
    Object* buttonExitText;

public:
    
    void OnEnter() override {
        AM.PlaySong("menuMusic");
        textMenu = new TextObject("MAIN MENU");
        textMenu->GetTransform()->position = Vector2(windowWidth / 2 - 25.0f, windowHeight / 2 - 150);
        SPAWN.SpawnObject(textMenu);

        //Games
        buttonSpaceInvaders = new Button(Vector2(windowWidth / 2 - 125.0f, windowHeight / 2 - 100), Vector2(250, 50), sf::Color(255, 0, 255, 255), "SPACE INVADERS", Button::ActionType::ChangeScene, "Space Invaders");

        buttonAudio = new Button(Vector2(windowWidth / 2 - 125.0f, windowHeight / 2 + 170), Vector2(250, 50), sf::Color(255, 0, 255, 255), "Toggle Audio", Button::ActionType::ToggleAudio);
        buttonExit = new Button(Vector2(windowWidth / 2 - 125.0f, windowHeight / 2 + 260), Vector2(250, 50), sf::Color(255, 0, 255, 255), "Exit Game", Button::ActionType::ExitGame);

        SPAWN.SpawnObject(buttonSpaceInvaders);
        SPAWN.SpawnObject(buttonAudio);
        SPAWN.SpawnObject(buttonExit);

        buttonSpaceInvadersText = new TextObject("TIC TAC TOE");
        buttonSpaceInvadersText->GetTransform()->position = Vector2(windowWidth / 2 - 30.0f, windowHeight / 2 - 40);
        SPAWN.SpawnObject(buttonSpaceInvadersText);

        buttonAudioText = new TextObject("EN/DISABLE AUDIO");
        buttonAudioText->GetTransform()->position = Vector2(windowWidth / 2 - 65.0f, windowHeight / 2 + 230);
        SPAWN.SpawnObject(buttonAudioText);

        buttonExitText = new TextObject("EXIT");
        buttonExitText->GetTransform()->position = Vector2(windowWidth / 2 + 25.0f, windowHeight / 2 + 320);
        SPAWN.SpawnObject(buttonExitText);
    }

    ~MainMenu()
    {
        buttonSplatText->Destroy();
        buttonSplatText = nullptr;

        buttonSpaceInvadersText->Destroy();
        buttonSpaceInvadersText = nullptr;

        buttonTanksText->Destroy();
        buttonTanksText = nullptr;

        textMenu->Destroy();
        textMenu = nullptr;

        buttonSpriteText->Destroy();
        buttonSpriteText = nullptr;

        buttonRankingText->Destroy();
        buttonRankingText = nullptr;
        
        buttonAudioText->Destroy();
        buttonAudioText = nullptr;
        
        buttonExitText->Destroy();
        buttonExitText = nullptr;
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
