#pragma once
#include "Object.h"
#include "RenderManager.h"
#include "InputManager.h"
#include "TextObject.h"
#include <cctype>

class TextField : public Object {
private:
    std::string content = "";
    Vector2 size;
    bool isFocused = false;
    bool prevLeftClick = false; // Used for click edge detection

    TextObject* visualText;
    sf::Color baseColor = sf::Color(50, 50, 50, 255);
    sf::Color focusColor = sf::Color(100, 100, 0, 255);

public:
    inline std::string GetText() const { return content; }

    TextField(Vector2 pos, Vector2 size) : size(size) {
        transform->position = pos;

        visualText = new TextObject("CODIGO...");
        visualText->GetTransform()->position = pos + Vector2(10, 10);
    }

    ~TextField() {
        delete visualText;
    }

    void Update() override {
        float mx = (float)Input.GetMouseX();
        float my = (float)Input.GetMouseY();
        bool currentClick = Input.GetLeftClick();

        // Update focus state based on mouse bounds and click
        if (currentClick && !prevLeftClick) {
            isFocused = (mx >= transform->position.x && mx <= transform->position.x + size.x &&
                my >= transform->position.y && my <= transform->position.y + size.y);

            if (isFocused && content == "") visualText->SetText("");
        }
        prevLeftClick = currentClick;

        if (isFocused) {
            std::string buffer = Input.GetTextBuffer();
            bool textChanged = false;

            // Process characters from the input buffer
            for (char c : buffer) {
                if (c == '\b') { // Backspace handling
                    if (!content.empty()) {
                        content.pop_back();
                        textChanged = true;
                    }
                }
                // Filter: printable characters, 8-char limit, preserve case (allow lowercase and uppercase)
                else if (c >= 33 && c <= 126 && content.length() < 8) {
                    content += c;
                    textChanged = true;
                }
            }

            if (textChanged) {
                visualText->SetText(content);
            }

            if (content == "") visualText->SetText("...");
        }
        else if (content == "") {
            // Restore placeholder when focus is lost
            visualText->SetText("CODIGO...");
        }

        visualText->Update();
    }

    void Render() override {
        // Draw background box with color feedback based on focus
        RM->DrawRect((int)transform->position.x, (int)transform->position.y,
            (int)size.x, (int)size.y,
            isFocused ? focusColor.r : baseColor.r,
            isFocused ? focusColor.g : baseColor.g,
            isFocused ? focusColor.b : baseColor.b, 255);

        visualText->Render();
    }

    std::string GetContent() { return content; }
};