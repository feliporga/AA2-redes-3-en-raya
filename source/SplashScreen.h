#pragma once
#include "Scene.h"
#include "SceneManager.h"
#include "TimeManager.h"
#include "RenderManager.h"
#include "ImageObject.h"
#include "TextObject.h"
#include <iostream>

class SplashScreen : public Scene
{
private:
    float timer = 0.0f;
    float splashDuration = 3.0f;
    ImageObject* splashImage = nullptr;
    TextObject* splashScreenText = nullptr;

public:
    SplashScreen() : Scene() {}

    void OnEnter() override {

        splashImage = new ImageObject("resources/spashscreen.png", Vector2(0, 0), Vector2(1200, 802), 1200, 802, 0, false);

        SPAWN.SpawnObject(splashImage);
        splashImage->GetTransform()->scale = Vector2(7.0f, 5.5f);
        float wW = RM->WINDOW_WIDTH;
        float wH = RM->WINDOW_HEIGHT;

        splashImage->GetTransform()->position = Vector2((wW/2), (wH / 2));
        

        splashScreenText = new TextObject("Splash Screen");
        SPAWN.SpawnObject(splashScreenText);
        splashScreenText->GetTransform()->position = Vector2((wW/2) - 50, 90);

        timer = 0.0f;
    }


    void Update() override {
        Scene::Update();

        timer += TIME.GetDeltaTime();

        if (timer >= splashDuration) {
            SM.SetNextScene("MainMenu");
        }
    }

    void Render() override {
        Scene::Render();
    }

    void OnExit() override {
        Scene::OnExit();
    }
};
