#pragma once
#include <memory>
#include <map>
#include "ComponentTypeID.h"

namespace ECS
{
class Entity;
using EntityPtr = std::shared_ptr<Entity>;
using EntityID = unsigned int;

class Entity
{
public:
	explicit Entity(EntityID entityID);
	~Entity();

	bool IsActive() const;
	bool HasComponent(const ComponentID index) const;
	
	template <typename TComponent, typename... TArgs>
	inline TComponent* AddComponent(TArgs&&... args);

	template<typename TComponent>
	inline TComponent* GetComponent() const;

private:
	EntityID entityID{0};
	bool active = true;

	std::map<ComponentID, IComponent*> components;
};

template<typename TComponent, typename ...TArgs>
inline TComponent* Entity::AddComponent(TArgs && ...args)
{
	if (!active)
	{
		throw std::runtime_error("Error, cannot add component to entity, entity has already been destroyed.");
	}

	if (HasComponent(ComponentTypeID::Get<TComponent>()))
	{
		throw std::runtime_error("Error, cannot add component to entity, component already exists");
	}
	
	return CreateComponent<TComponent>(std::forward<TArgs>(args)...);
}

template<typename T>
inline T * Entity::GetComponent() const
{
	ComponentID index = ComponentTypeID::Get<T>();
	if (!HasComponent(index))
	{
		throw std::runtime_error("Error, cannot get component from entity, component not exists");
	}
	auto ptr = components.at(index);
	return static_cast<T*>(ptr);
}
}