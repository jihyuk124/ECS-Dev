#pragma once
#include <stack>
#include <map>
#include <unordered_set>
#include "Entity.h"

namespace ECS
{
class Entity;
using ComponentID = unsigned int;
class EntityPool
{
public:
	EntityPool();
	~EntityPool();

	Entity * CreateEntity();
	void Destroy();
	void Remove(Entity* entity);
	//void RemoveAll();

private:
	std::unordered_set<Entity*> entities;
	std::stack<Entity*> reusableEntities;

	std::map<ComponentID, std::stack<IComponent*>> componentPools;

	unsigned int counter;
};
}
