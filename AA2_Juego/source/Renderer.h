#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include <string>

#include "Transform.h"

class Renderer {
protected:
	SDL_Color color;
	Transform* transform;
	SDL_Rect destRect;
	SDL_Rect sourceRect;
	std::string targetPath;

public:
	Renderer(Transform* transform, std::string targetPath) {
		this->transform = transform;
		this->targetPath = targetPath;
	}
	~Renderer() {}
		
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void SetColor(SDL_Color color) {
		this->color = color;
	};
	SDL_Color GetColor() { return color; }
};