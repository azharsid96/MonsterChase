#pragma once
#include "Math/Vector2/Point2D.h"
#include "Math/Vector3/Vector3.h"
#include "GLib.h"
#include "../SmartPtrs/SmartPtrs.h"

#include <string>

class GameObject
{
public:
	GameObject(Engine::Math::Vector3 const  i_position, std::string i_name, float i_ZRotation);
	GameObject();
	~GameObject();

	//static inline SmartPtrs<GameObject> CreateGameObject(Engine::Math::Point2D const & i_position, std::string const & i_name) { return SmartPtrs<GameObject>(new GameObject(i_position, i_name)); };

	inline std::string GetName() const { return m_name; };

	inline Engine::Math::Vector3 GetPosition() const { return m_position; };

	inline Engine::Math::Vector3 GetVelocity() const { return m_velocity; };

	inline float GetRotation() const { return m_ZRotation; };

	inline float GetMass() const { return m_mass; };

	inline void SetMass(float const & i_mass) { m_mass = i_mass; };

	inline void SetPosition(Engine::Math::Vector3 const & i_position) { m_position = i_position; };

	inline void SetVelocity(Engine::Math::Vector3 const & i_velocity) { m_velocity = i_velocity; };

	inline void SetBBCenter(Engine::Math::Point2D const & i_BBCenter) { m_BBCenter = i_BBCenter; };

	inline Engine::Math::Point2D GetBBCenter() { return m_BBCenter; };

	inline void SetExtents(Engine::Math::Point2D const & i_Extents) { m_Extents = i_Extents; };

	inline Engine::Math::Point2D GetExtents() { return m_Extents; };


private:
	
	Engine::Math::Vector3					m_position;
	Engine::Math::Vector3					m_orientation;
	Engine::Math::Vector3					m_velocity;
	float									m_ZRotation;
	float									m_mass;
	std::string 							m_name;
	Engine::Math::Point2D					m_BBCenter;
	Engine::Math::Point2D					m_Extents;
};