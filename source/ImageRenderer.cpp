#include "ImageRenderer.h"
#include "RenderManager.h"
#include "TimeManager.h"

ImageRenderer::ImageRenderer(Transform* transform, std::string resourcePath, Vector2 sourceOffset, Vector2 sourceSize)
	: Renderer(transform, resourcePath) 
{
	RM->GetTexture(resourcePath);

	sourceRect = SDL_Rect{ //(int) porq hacemos referencia a pixeles
		(int)sourceOffset.x,
		(int)sourceOffset.y,
		(int)sourceSize.x,
		(int)sourceSize.y
	};

	destRect = SDL_Rect{
		(int)transform->position.x,
		(int)transform->position.y,
		(int)transform->scale.x,
		(int)transform->scale.y,
	};	
}

void ImageRenderer::Update()
{
	
	Vector2 offset = (Vector2(-transform->size.x, -transform->size.y) / 2.0f) * transform->scale;

	destRect.x = transform->position.x + offset.x;
	destRect.y = transform->position.y + offset.y;

	destRect.w = transform->size.x * transform->scale.x;
	destRect.h = transform->size.y * transform->scale.y;
}

void ImageRenderer::Render()
{
	//SDL_RenderCopy(RM->GetRenderer(), RM->GetTexture(targetPath), &sourceRect, &destRect);


	SDL_RenderCopyEx(
		RM->GetRenderer(),
		RM->GetTexture(targetPath),
		&sourceRect,
		&destRect,
		transform->rotation,
		NULL,
		SDL_FLIP_NONE);
}
