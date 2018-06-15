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
		newEntity = new Entity(&componentPools);
		entities.insert(newEntity);
	}
	newEntity->entityID = ++entityCounter;
	return newEntity;
}

void EntityPool::Destroy()
{
	//Entity 삭제
	for (const auto &entity : entities)
	{
		entity->Destroy();
		delete entity;
	}
	entities.clear();

	while (false == reusableEntities.empty())
	{
		reusableEntities.top()->Destroy();
		delete reusableEntities.top();
		reusableEntities.pop();
	}

	//Component 삭제
	for (auto &pair : componentPools)
	{
		auto componentPool = pair.second;

		while (!componentPool.empty())
		{
			delete componentPool.top();
			componentPool.pop();
		}
	}
}

void EntityPool::RemoveEntity(Entity * entity)
{
	auto removed = entities.erase(entity);
	if (!removed)
	{
		throw std::runtime_error("Error, cannot destroy entity. Pool does not contain entity.");
	}

	reusableEntities.push(entity);
}

void EntityPool::RemoveAllEntity(Entity * entity)
{
}

}