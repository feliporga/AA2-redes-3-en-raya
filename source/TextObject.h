#pragma once
#include "Object.h"
#include "TextRenderer.h"

class TextObject : public Object
{
private:
	TextRenderer* tr;

public:
	TextObject(std::string text) : Object() {
		tr = new TextRenderer(transform, text);
		renderer = tr;
		tr->SetColor(SDL_Color{ 0xFF, 0xFF, 0xFF, 0xFF });
	}

	~TextObject() {
		if (tr != nullptr) {
			delete tr;
			tr = nullptr;
		}
	}

	void SetText(std::string text) {
		tr->SetText(text);
	}
};