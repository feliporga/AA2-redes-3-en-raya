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


class SpriteMenu : public Scene
{
private:
    float windowWidth = RM->WINDOW_WIDTH;
    float windowHeight = RM->WINDOW_HEIGHT;

    Object* textSprite;

    Button* sqr11;
    Button* sqr12;
    Button* sqr13;

    Button* sqr21;
    Button* sqr22;
    Button* sqr23;

    Button* sqr31;
    Button* sqr32;
    Button* sqr33;

    Button* buttonBack;

    Object* backgroundText;
    Object* sqr11text;
    Object* sqr12text;
    Object* sqr13text;

    Object* enemiesText;
    Object* sqr21text;
    Object* sqr22text;
    Object* sqr23text;

    Object* vfxText;
    Object* sqr31text;
    Object* sqr32text;
    Object* sqr33text;

    Object* buttonBacktext;

public:
    void OnEnter() override {
        AM.PlaySong("spriteMenu");
        textSprite = new TextObject("SELECT SPRITES");
        textSprite->GetTransform()->position = Vector2(windowWidth / 2 - 50.0f, windowHeight / 2 - 150);
        SPAWN.SpawnObject(textSprite);

        //background
        sqr11 = new Button(Vector2(windowWidth / 2 - 150.0f, windowHeight / 2 - 100), Vector2(50, 50), SDL_Color{ 255, 0, 255, 255 }, "11", Button::ActionType::SpritesChange);
        sqr12 = new Button(Vector2(windowWidth / 2 - 50.0f, windowHeight / 2 - 100), Vector2(50, 50), SDL_Color{ 255, 0, 255, 255 }, "12", Button::ActionType::SpritesChange);
        sqr13 = new Button(Vector2(windowWidth / 2 + 50.0f, windowHeight / 2 - 100), Vector2(50, 50), SDL_Color{ 255, 0, 255, 255 }, "13", Button::ActionType::SpritesChange);
        
        //enemies
        sqr21 = new Button(Vector2(windowWidth / 2 - 150.0f, windowHeight / 2 - 10), Vector2(50, 50), SDL_Color{ 255, 0, 255, 255 }, "21", Button::ActionType::SpritesChange);
        sqr22 = new Button(Vector2(windowWidth / 2 - 50.0f, windowHeight / 2 - 10), Vector2(50, 50), SDL_Color{ 255, 0, 255, 255 }, "22", Button::ActionType::SpritesChange);
        sqr23 = new Button(Vector2(windowWidth / 2 + 50.0f, windowHeight / 2 - 10), Vector2(50, 50), SDL_Color{ 255, 0, 255, 255 }, "23", Button::ActionType::SpritesChange);

        //vfx
        sqr31 = new Button(Vector2(windowWidth / 2 - 150.0f, windowHeight / 2 + 80), Vector2(50, 50), SDL_Color{ 255, 0, 255, 255 }, "31", Button::ActionType::SpritesChange);
        sqr32 = new Button(Vector2(windowWidth / 2 - 50.0f, windowHeight / 2 + 80), Vector2(50, 50), SDL_Color{ 255, 0, 255, 255 }, "32", Button::ActionType::SpritesChange);
        sqr33 = new Button(Vector2(windowWidth / 2 + 50.0f, windowHeight / 2 + 80), Vector2(50, 50), SDL_Color{ 255, 0, 255, 255 }, "33", Button::ActionType::SpritesChange);


        buttonBack = new Button(Vector2(50, windowHeight - 100), Vector2(250, 50), SDL_Color{ 255, 0, 255, 255 }, "MainMenu", Button::ActionType::ChangeScene, "MainMenu");

        SPAWN.SpawnObject(sqr11);
        SPAWN.SpawnObject(sqr12);
        SPAWN.SpawnObject(sqr13);
        SPAWN.SpawnObject(sqr21);
        SPAWN.SpawnObject(sqr22);
        SPAWN.SpawnObject(sqr23);
        SPAWN.SpawnObject(sqr31);
        SPAWN.SpawnObject(sqr32);
        SPAWN.SpawnObject(sqr33);
        SPAWN.SpawnObject(buttonBack);

        backgroundText = new TextObject("Background");
        backgroundText->GetTransform()->position = Vector2(windowWidth / 2 - 430.0f, windowHeight / 2 - 40);
        SPAWN.SpawnObject(backgroundText);
        
        sqr11text = new TextObject("1");
        sqr11text->GetTransform()->position = Vector2(windowWidth / 2 - 80.0f, windowHeight / 2 - 40);
        SPAWN.SpawnObject(sqr11text);
        
        sqr12text = new TextObject("2");
        sqr12text->GetTransform()->position = Vector2(windowWidth / 2 + 15.0f, windowHeight / 2 - 40);
        SPAWN.SpawnObject(sqr12text);

        sqr13text = new TextObject("3");
        sqr13text->GetTransform()->position = Vector2(windowWidth / 2 + 115.0f, windowHeight / 2 - 40);
        SPAWN.SpawnObject(sqr13text);


        enemiesText = new TextObject("Enemies");
        enemiesText->GetTransform()->position = Vector2(windowWidth / 2 - 430.0f, windowHeight / 2 + 50);
        SPAWN.SpawnObject(enemiesText);
        
        sqr21text = new TextObject("1");
        sqr21text->GetTransform()->position = Vector2(windowWidth / 2 - 80.0f, windowHeight / 2 + 50);
        SPAWN.SpawnObject(sqr21text);
        
        sqr22text = new TextObject("2");
        sqr22text->GetTransform()->position = Vector2(windowWidth / 2 + 15.0f, windowHeight / 2 + 50);
        SPAWN.SpawnObject(sqr22text);
        
        sqr23text = new TextObject("3");
        sqr23text->GetTransform()->position = Vector2(windowWidth / 2 + 115.0f, windowHeight / 2 + 50);
        SPAWN.SpawnObject(sqr23text);


        vfxText = new TextObject("VFX");
        vfxText->GetTransform()->position = Vector2(windowWidth / 2 - 430.0f, windowHeight / 2 + 150);
        SPAWN.SpawnObject(vfxText);
        
        sqr31text = new TextObject("1");
        sqr31text->GetTransform()->position = Vector2(windowWidth / 2 - 80.0f, windowHeight / 2 + 140);
        SPAWN.SpawnObject(sqr31text);
        
        sqr32text = new TextObject("2");
        sqr32text->GetTransform()->position = Vector2(windowWidth / 2 + 15.0f, windowHeight / 2 + 140);
        SPAWN.SpawnObject(sqr32text);
        
        sqr33text = new TextObject("3");
        sqr33text->GetTransform()->position = Vector2(windowWidth / 2 + 115.0f, windowHeight / 2 + 140);
        SPAWN.SpawnObject(sqr33text);

        
        buttonBacktext = new TextObject("BACK");
        buttonBacktext->GetTransform()->position = Vector2(195, windowHeight - 40);
        SPAWN.SpawnObject(buttonBacktext);
    }

    ~SpriteMenu()
    {
        textSprite->Destroy();
        textSprite = nullptr;

        backgroundText->Destroy();
        backgroundText = nullptr;
        sqr11text->Destroy();
        sqr11text = nullptr;
        sqr12text->Destroy();
        sqr12text = nullptr;
        sqr13text->Destroy();
        sqr13text = nullptr;
        
        enemiesText->Destroy();
        enemiesText = nullptr;
        sqr21text->Destroy();
        sqr21text = nullptr;
        sqr22text->Destroy();
        sqr22text = nullptr;
        sqr23text->Destroy();
        sqr23text = nullptr;
        
        vfxText->Destroy();
        vfxText = nullptr;
        sqr31text->Destroy();
        sqr31text = nullptr;
        sqr32text->Destroy();
        sqr32text = nullptr;
        sqr33text->Destroy();
        sqr33text = nullptr;
        
        buttonBacktext->Destroy();
        buttonBacktext = nullptr;
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