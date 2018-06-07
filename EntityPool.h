#pragma once
#include "IPool.h"
#include <stack>
#include <unordered_set>

namespace ECS
{
class Entity;
class EntityPool
{
public:
	EntityPool();
	~EntityPool();

	Entity * CreateEntity();
	void Destroy();
	void Remove(Entity* entity);

private:
	std::unordered_set<Entity*> entities;
	std::stack<Entity*> reusableEntities;
	unsigned int counter = 0;
};
}
