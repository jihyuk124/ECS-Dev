#include "EntityPool.h"
#include "Entity.h"
#include <algorithm>

namespace ECS
{
EntityPool::EntityPool()
{
	entityCounter = 0;
	OnEntityReleasedCache = std::bind(&EntityPool::OnEntityReleased, this, std::placeholders::_1);
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
		newEntity->isActive = true;
	}
	else
	{
		newEntity = new Entity(&componentPools);
		if (!newEntity)
		{
			throw std::runtime_error("Error, cannot create entity.");
		}
		newEntity->entityID = entityCounter++;
	}

	entities.insert(newEntity);
	entitiesCache.clear();

	newEntity->OnComponentAdded += [this](Entity* entity, ComponentID index, IComponent* component)
	{
		UpdateGroupsComponentAddedOrRemoved(entity, index, component);
	};
	newEntity->OnComponentRemoved += [this](Entity* entity, ComponentID index, IComponent* component)
	{
		UpdateGroupsComponentAddedOrRemoved(entity, index, component);
	};
	newEntity->OnComponentReplaced += [this](Entity* entity, ComponentID index, IComponent* previousComponent, IComponent* newComponent)
	{
		UpdateGroupsComponentReplaced(entity, index, previousComponent, newComponent);
	};

	newEntity->OnEntityReleased.Clear();
	newEntity->OnEntityReleased += OnEntityReleasedCache;

	OnEntityCreated(this, newEntity);

	return newEntity;
}

void EntityPool::Destroy()
{
	//카운터 초기화
	entityCounter = 0;

	//Group 삭제
	for (auto &it : groups)
	{
		it.second->RemoveAllEventHandlers();
		OnGroupCleared(this, it.second);
		delete it.second;
	}
	groups.clear();

	for (auto &pair : groupsForIndex)
	{
		pair.second.clear();
	}
	groupsForIndex.clear();

	//Entity 삭제
	auto entitiesTemp = std::vector<Entity*>(entities.begin(), entities.end());

	while (!entities.empty())
	{
		RemoveEntity(entitiesTemp.back());
		entitiesTemp.pop_back();
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
	
	entitiesCache.clear();
	
	OnEntityWillBeDestroyed(this, entity);
	entity->Destroy();
	OnEntityDestroyed(this, entity);

	entity->OnEntityReleased -= OnEntityReleasedCache;	
}

bool EntityPool::HasEntity(Entity *& entity) const
{
	return std::find(entities.begin(), entities.end(), entity) != entities.end();
}

auto EntityPool::GetEntities() -> std::vector<Entity *>
{
	if (entitiesCache.empty())
	{
		entitiesCache = std::vector<Entity*>(entities.begin(), entities.end());
	}

	return entitiesCache;
}

auto EntityPool::GetEntities(const Matcher matcher) -> std::vector<Entity *>
{
	return GetGroup(std::move(matcher))->GetEntities();
}

auto EntityPool::GetGroup(Matcher matcher) -> Group *
{
	Group* group = nullptr;

	auto it = groups.find(matcher);

	if (it == groups.end())
	{
		group = new Group(matcher);

		auto entities = GetEntities();

		for (int i = 0, entitiesLength = entities.size(); i < entitiesLength; i++)
		{
			group->HandleEntitySilently(entities[i]);
		}

		groups[group->GetMatcher()] = group;

		for (int i = 0, indicesLength = matcher.GetIndices().size(); i < indicesLength; i++)
		{
			groupsForIndex[matcher.GetIndices()[i]].push_back(group);
		}

		OnGroupCreated(this, group);
	}
	else
	{
		group = it->second;
	}

	return group;
}

auto EntityPool::GetEntityCount() const -> unsigned int
{
	return entities.size();
}

auto EntityPool::GetReusableEntitiesCount() const -> unsigned int
{
	return reusableEntities.size();
}

void EntityPool::UpdateGroupsComponentAddedOrRemoved(Entity * entity, ComponentID index, IComponent * component)
{
	if (groupsForIndex.find(index) == groupsForIndex.end())
	{
		return;
	}

	auto tmpGroups = groupsForIndex[index];

	if (tmpGroups.size() > 0)
	{
		auto events = std::vector<Group::GroupChanged*>();

		for (int i = 0, groupsCount = tmpGroups.size(); i < groupsCount; ++i)
		{
			events.push_back(tmpGroups[i]->HandleEntity(entity));
		}

		for (int i = 0, eventsCount = events.size(); i < eventsCount; ++i)
		{
			(*events[i])(tmpGroups[i], entity, index, component);
		}
	}
}

void EntityPool::UpdateGroupsComponentReplaced(Entity * entity, ComponentID index, IComponent * previousComponent, IComponent * newComponent)
{
	if (groupsForIndex.find(index) == groupsForIndex.end())
	{
		return;
	}

	if (groupsForIndex[index].size() > 0)
	{
		for (const auto &g : groupsForIndex[index])
		{
			g->UpdateEntity(entity, index, previousComponent, newComponent);
		}
	}
}

void EntityPool::OnEntityReleased(Entity * entity)
{
	if (entity->isActive)
	{
		throw std::runtime_error("Error, cannot release entity. Entity is not destroyed yet.");
	}

	entities.erase(entity);
	reusableEntities.push(entity);
}

}