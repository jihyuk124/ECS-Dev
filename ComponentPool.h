#pragma once
#include "IPool.h"
#include "IComponent.h"
#include "ComponentTypeID.h"
#include <map>
#include <stack>

namespace ECS
{
class IComponent;
class ComponentPool
{
public:
	ComponentPool();
	~ComponentPool();

	template <typename TComponent, typename... TArgs>
	inline TComponent* CreateComponent(TArgs&&... args);
	void Destroy();

private:
	std::map<ComponentID, std::stack<IComponent*>> components;
	std::map<ComponentID, std::stack<IComponent*>> reusableComponents;
};

template<typename TComponent, typename ...TArgs>
inline TComponent * ComponentPool::CreateComponent(TArgs && ...args)
{
	TComponent* newComponent = nullptr;
	ComponentID id = ComponentTypeID::Get<TComponent>();
	
	//키값이 없으면 디폴트 스택 생성
	if (reusableComponents[id].size() > 0)
	{
		newComponent = reusableComponents[id].top();
		reusableComponents[id].pop();
	}
	else
	{
		newComponent = new TComponent(std::forward<TArgs>(args)...);
		components[id].push(newComponent);
	}
	return newComponent;
}

//
//template<typename T, typename ...TArgs>
//inline T* Entity::CreateComponent(TArgs && ...args)
//{
//	T* newComponent(new T(std::forward<TArgs>(args)...));
//	newComponent->Reset(std::forward<TArgs>(args)...);
//	std::unique_ptr<IComponent> uPtr{ newComponent };
//	components[ComponentTypeID::Get<T>()] = uPtr;
//	return newComponent;
//}
}