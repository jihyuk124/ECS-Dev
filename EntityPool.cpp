#include "EntityPool.h"
#include "Entity.h"

namespace ECS
{
EntityPool::EntityPool()
{
}

EntityPool::~EntityPool()
{
	Destroy();
}

Entity * EntityPool::CreateEntity()
{
	Entity * newEntity = nullptr;
	if (false == reusableEntities.empty())
	{
		newEntity = reusableEntities.top();
		reusableEntities.pop();
	}
	else
	{
		newEntity = new Entity(++counter);
		entities.insert(newEntity);
	}
	return newEntity;
}

void EntityPool::Destroy()
{
	for (const auto &entity : entities)
	{
		delete entity;
	}
	entities.clear();

	while (false == reusableEntities.empty())
	{
		delete reusableEntities.top();
		reusableEntities.pop();
	}
}

void EntityPool::Remove(Entity * entity)
{
	auto removed = entities.erase(entity);
	if (!removed)
	{
		throw std::runtime_error("Error, cannot destroy entity. Pool does not contain entity.");
	}

	reusableEntities.push(entity);
}

}