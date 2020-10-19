#include "GameObject.h"

GameObject::GameObject(){}

GameObject::GameObject(Engine::Math::Vector3 const  i_position, std::string i_name, float i_ZRotation) :
m_position(i_position), m_name(i_name), m_ZRotation(i_ZRotation) {
	m_velocity = Engine::Math::Vector3::Zero;
}

GameObject::~GameObject() {
}

