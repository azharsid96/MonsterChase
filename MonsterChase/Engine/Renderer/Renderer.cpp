#include "Renderer.h"

Engine::Mutex mutexRendererLock;

Renderer * Renderer::r_Instance = new Renderer();

bool Renderer::AddGameObjectToRenderer(SmartPtrs<GameObject> i_game_object, std::string game_obj_sprite) {

	assert(i_game_object);

	float width = 0;
	float height = 0;
	
	GLib::Sprite * sprite = Helpers::CreateSprite(game_obj_sprite.data(), width, height);

	assert((width > 0) && (height > 0));

	//this info will be used by the collision system
	i_game_object->SetBBCenter(Engine::Math::Point2D(.0f, height / 2.0f));
	i_game_object->SetExtents(Engine::Math::Point2D(width / 2.0f, height / 2.0f));

	Engine::ScopeLock scopeLock(mutexRendererLock);
	
	Renderer::r_Instance->newObjsToRender.emplace_back(WeakPtrs<GameObject>(i_game_object), sprite);

	return true;
}

void Renderer::MoveObjects()
{
	Engine::ScopeLock scopeLock(mutexRendererLock);
	
	for (int i = 0; i < r_Instance->newObjsToRender.size(); ++i)
	{
		r_Instance->objsToRender.push_back(r_Instance->newObjsToRender[i]);
	}

	r_Instance->newObjsToRender.clear();
}