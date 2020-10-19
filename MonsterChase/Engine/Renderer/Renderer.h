#pragma once
#include "GLib.h"
#include "Helpers/Helpers.h"
#include "GameCommon/GameObject/GameObject.h"
#include "../GameCommon/WeakPtrs/WeakPtrs.h"
#include "Syncronization/Mutex.h"
#include "Syncronization/ScopeLock.h"

#include <DirectXColors.h>

#include "assert.h"
#include "vector"

struct Renderable
{
	WeakPtrs<GameObject> obj;
	GLib::Sprite * spriteData;
	GLib::Point2D Offset;

	Renderable(WeakPtrs<GameObject> i_game_object, GLib::Sprite * i_spriteData) :
	obj(i_game_object), spriteData(i_spriteData)
	{		
		Offset.x = i_game_object.Acquire()->GetPosition().x();
		Offset.y = i_game_object.Acquire()->GetPosition().y();
	}
};

class Renderer
{
public:
	inline static void Initialize() { /*Renderer::r_Instance->objsToRender = std::vector<Renderable>(); */};
	
	static bool AddGameObjectToRenderer(SmartPtrs<GameObject> i_game_object, std::string game_obj_sprite);

	static void MoveObjects();

	//may require delta time param
	inline static void Draw()
	{
		MoveObjects();
		
		GLib::BeginRendering(DirectX::Colors::Brown);
		// Tell GLib that we want to render some sprites
		GLib::Sprites::BeginRendering();
		
		for (int i=0; i < Renderer::r_Instance->objsToRender.size(); ++i)
		{
			r_Instance->objsToRender[i].Offset.x = r_Instance->objsToRender[i].obj.Acquire()->GetPosition().x();
			r_Instance->objsToRender[i].Offset.y = r_Instance->objsToRender[i].obj.Acquire()->GetPosition().y();
			
			GLib::Render(*(Renderer::r_Instance->objsToRender[i].spriteData), Renderer::r_Instance->objsToRender[i].Offset, 
				r_Instance->objsToRender[i].obj.Acquire()->GetRotation());
		}
		
		// Tell GLib we're done rendering sprites
		GLib::Sprites::EndRendering();
		// IMPORTANT: Tell GLib we're done rendering
		GLib::EndRendering();
	}

	inline static void Shutdown()
	{
		for (int i = 0; i < Renderer::r_Instance->objsToRender.size(); ++i)
		{
			GLib::Release(Renderer::r_Instance->objsToRender[i].spriteData);
		}
		
		delete Renderer::r_Instance;
	}
	

private:
	static Renderer*			r_Instance;
	Renderer(){}
	std::vector<Renderable>		objsToRender;
	std::vector<Renderable>		newObjsToRender;
};