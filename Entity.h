#pragma once
#include <memory>
#include <stack>
#include <map>
#include "ComponentTypeID.h"
#include "Delegate.h"

namespace ECS
{
class Entity;
using EntityID = unsigned int;

class Entity
{
//함수
public:
	friend class EntityPool;
	Entity(std::map<ComponentID, std::stack<IComponent*>>* componentPools);
	~Entity();

	template <typename T, typename... TArgs> inline Entity* Add(TArgs&&... args);
	template <typename T> inline Entity* Remove();
	template <typename T, typename... TArgs> inline Entity* Replace(TArgs&&... args);
	template <typename T> inline Entity* Refresh();
	template<typename TComponent>inline TComponent* Get() const;
	template <typename T> inline T* Use();
	template <typename T> inline bool Has() const;

	bool IsActive() const;
	//bool HasComponents(const std::vector<ComponentID>& indices) const;
	//bool HasAnyComponent(const std::vector<ComponentID>& indices) const;
	//unsigned int GetComponentsCount() const;
	void RemoveAllComponents();

	bool operator ==(const Entity*& right) const;

private:
	template <typename T, typename... TArgs> inline IComponent* CreateComponent(TArgs&&... args);

	Entity* AddComponent(ComponentID index, IComponent* component);
	Entity* RemoveComponent(ComponentID index);
	Entity* ReplaceComponent(const ComponentID index, IComponent* component);
	IComponent* GetComponent(const ComponentID index) const;
	bool HasComponent(const ComponentID index) const;
	void Destroy();

	std::stack<IComponent*>* GetComponentPool(const ComponentID index) const;
	void Replace(const ComponentID index, IComponent* replacement);

//변수
public:
	using EntityChanged = Delegate<void(Entity* entity, ComponentID index, IComponent* component)>;
	using ComponentReplaced = Delegate<void(Entity* entity, ComponentID index, IComponent* previousComponent, IComponent* newComponent)>;
	using EntityReleased = Delegate<void(Entity* entity)>;

	EntityChanged OnComponentAdded;
	ComponentReplaced OnComponentReplaced;
	EntityChanged OnComponentRemoved;
	EntityReleased OnEntityReleased;

private:
	EntityID entityID{0};
	bool active = true;

	std::map<ComponentID, IComponent*> components;
	std::map<ComponentID, std::stack<IComponent*>>* componentPools;
};

template<typename T, typename ...TArgs>
inline Entity* Entity::Add(TArgs && ...args)
{
	return AddComponent(ComponentTypeID::Get<T>(), CreateComponent<T>(std::forward<TArgs>(args)...));
}
template<typename T>
inline Entity * Entity::Remove()
{
	return RemoveComponent(ComponentTypeID::Get<T>());
}
template<typename T, typename ...TArgs>
inline Entity * Entity::Replace(TArgs && ...args)
{
	return ReplaceComponent(ComponentTypeID::Get<T>(), CreateComponent<T>(std::forward<TArgs>(args)...));
}
template<typename T>
inline Entity * Entity::Refresh()
{
	return ReplaceComponent(ComponentTypeID::Get<T>(), Get<T>());
}

template<typename T>
inline T * Entity::Get() const
{
	return static_cast<T*>(GetComponent(ComponentTypeID::Get<T>()));
}

template<typename T>
inline T * Entity::Use()
{
	Refresh<T>();
	return static_cast<T*>(GetComponent(ComponentTypeID::Get<T>()));
}

template<typename T>
inline bool Entity::Has() const
{
	return HasComponent(ComponentTypeID::Get<T>());
}

template<typename T, typename ...TArgs>
inline IComponent * Entity::CreateComponent(TArgs && ...args)
{
	std::stack<IComponent*>* componentPool = GetComponentPool(ComponentTypeID::Get<T>());
	IComponent* component = nullptr;

	if (componentPool->size() > 0)
	{
		component = componentPool->top();
		componentPool->pop();
	}
	else
	{
		component = new T();
	}

	(static_cast<T*>(component))->Reset(std::forward<TArgs>(args)...);

	return component;
}
}