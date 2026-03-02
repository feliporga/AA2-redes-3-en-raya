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
        buttonSpaceInvaders = new Button(Vector2(windowWidth / 2 - 475.0f, windowHeight / 2 - 100), Vector2(250, 50), SDL_Color{ 255, 0, 255, 255 }, "SPACE INVADERS", Button::ActionType::ChangeScene, "Space Invaders");
        buttonTanks = new Button(Vector2(windowWidth / 2 - 125.0f, windowHeight / 2 - 100), Vector2(250, 50), SDL_Color{ 255, 0, 255, 255 }, "TANKS", Button::ActionType::ChangeScene, "Tanks");
        buttonSplat = new Button(Vector2(windowWidth / 2 + 225.0f, windowHeight / 2 - 100), Vector2(250, 50), SDL_Color{ 255, 0, 255, 255 }, "SPLAT", Button::ActionType::ChangeScene, "Splat");
        
        //others
        buttonSprite = new Button(Vector2(windowWidth / 2 - 125.0f, windowHeight / 2 - 10), Vector2(250, 50), SDL_Color{ 255, 0, 255, 255 }, "SPRITE SELECTOR", Button::ActionType::ChangeScene, "SpriteMenu");
        buttonRanking = new Button(Vector2(windowWidth / 2 - 125.0f, windowHeight / 2 + 80), Vector2(250, 50), SDL_Color{ 255, 0, 255, 255 }, "RANKING SELECTOR", Button::ActionType::ChangeScene, "RankingMenu");

        buttonAudio = new Button(Vector2(windowWidth / 2 - 125.0f, windowHeight / 2 + 170), Vector2(250, 50), SDL_Color{ 255, 0, 255, 255 }, "Toggle Audio", Button::ActionType::ToggleAudio);
        buttonExit = new Button(Vector2(windowWidth / 2 - 125.0f, windowHeight / 2 + 260), Vector2(250, 50), SDL_Color{ 255, 0, 255, 255 }, "Exit Game", Button::ActionType::ExitGame);


        SPAWN.SpawnObject(buttonSpaceInvaders);
        SPAWN.SpawnObject(buttonTanks);
        SPAWN.SpawnObject(buttonSplat);
        SPAWN.SpawnObject(buttonSprite);
        SPAWN.SpawnObject(buttonRanking);
        SPAWN.SpawnObject(buttonAudio);
        SPAWN.SpawnObject(buttonExit);

        buttonSpaceInvadersText = new TextObject("SPACE INVADERS");
        buttonSpaceInvadersText->GetTransform()->position = Vector2(windowWidth / 2 - 405.0f, windowHeight / 2 - 40);
        SPAWN.SpawnObject(buttonSpaceInvadersText);

        buttonTanksText = new TextObject("TANKS");
        buttonTanksText->GetTransform()->position = Vector2(windowWidth / 2 + 5.0f, windowHeight / 2 - 40);
        SPAWN.SpawnObject(buttonTanksText);

        buttonSplatText = new TextObject("SPLAT");
        buttonSplatText->GetTransform()->position = Vector2(windowWidth / 2 + 360.0f, windowHeight / 2 - 40);
        SPAWN.SpawnObject(buttonSplatText);
        
        buttonSpriteText = new TextObject("SPRITE SELECTOR");
        buttonSpriteText->GetTransform()->position = Vector2(windowWidth / 2 - 60.0f, windowHeight / 2 + 50);
        SPAWN.SpawnObject(buttonSpriteText);

        buttonRankingText = new TextObject("CHECK RANKING");
        buttonRankingText->GetTransform()->position = Vector2(windowWidth / 2 - 50.0f, windowHeight / 2 + 140);
        SPAWN.SpawnObject(buttonRankingText);

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
