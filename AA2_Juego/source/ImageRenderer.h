#pragma once
#include "Renderer.h"

class ImageRenderer : public Renderer {
	//trsnaform para saber scale, position....
	//path: sprites
	//sourceOffset: dnd comienza  a print imagen en consola
	//sourceSize: tamaño textura/imagen
public:
	ImageRenderer(Transform* transform, std::string resourcePath, Vector2 sourceOffset, Vector2 sourceSize);
	~ImageRenderer() = default;

	virtual void Update() override;
	virtual void Render() override;

};