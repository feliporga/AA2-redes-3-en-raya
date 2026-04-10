#pragma once
#include "Scene.h"
#include "TextField.h"
#include "Button.h"
#include "TextObject.h"

class LoginScene : public Scene {
private:
    float winW;
    float winH;

    TextObject* titleText;

    TextField* userField;
    TextField* passField;

    Button* btnLogin;
    Button* btnRegister;

    TextObject* userLabel;
    TextObject* passLabel;
    TextObject* btnLoginText;
    TextObject* btnRegisterText;

public:
    LoginScene();
    ~LoginScene() = default;

    void OnEnter() override;
    void OnExit() override;
    void Update() override;
    void Render() override;
};