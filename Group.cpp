#include "Group.h"
#include "Matcher.h"
#include <algorithm>
namespace ECS
{

Group::Group(const Matcher& matcher) : matcher(matcher)
{
}

auto Group::Count() const -> const unsigned int
{
	return entities.size();
}

auto Group::GetEntities() -> std::vector<Entity *>
{
	if (entitiesCache.empty() && !entities.empty())
	{
		entitiesCache = std::vector<Entity*>(entities.begin(), entities.end());
	}

	return entitiesCache;
}

auto Group::GetSingleEntity() const -> Entity *
{
	auto count = Count();

	if (count == 1)
	{
		return *(entities.begin());
	}
	else if (count == 0)
	{
		return nullptr;
	}
	else
	{
		throw std::runtime_error("Error, cannot get the single entity from group. Group contains more than one entity.");
	}

	return nullptr;
}

bool Group::ContainsEntity(Entity *& entity) const
{
	return std::find(entities.begin(), entities.end(), entity) != entities.end();
}

auto Group::GetMatcher() const -> Matcher
{
	return matcher;
}

auto Group::HandleEntity(Entity * entity) -> GroupChanged *
{
	return matcher.Matches(entity) ? AddEntity(entity) : RemoveEntity(entity);
}

void Group::HandleEntitySilently(Entity * entity)
{
	if (matcher.Matches(entity))
	{
		AddEntitySilently(entity);
	}
	else
	{
		RemoveEntitySilently(entity);
	}
}

void Group::HandleEntity(Entity * entity, ComponentID index, IComponent * component)
{
	if (matcher.Matches(entity))
	{
		AddEntity(entity, index, component);
	}
	else
	{
		RemoveEntity(entity, index, component);
	}
}

void Group::UpdateEntity(Entity * entity, ComponentID index, IComponent * previousComponent, IComponent * newComponent)
{
	if (ContainsEntity(entity))
	{
		OnEntityRemoved(this, entity, index, previousComponent);
		OnEntityAdded(this, entity, index, newComponent);
		OnEntityUpdated(this, entity, index, previousComponent, newComponent);
	}
}

void Group::RemoveAllEventHandlers()
{
	OnEntityAdded.Clear();
	OnEntityRemoved.Clear();
	OnEntityUpdated.Clear();
}


bool Group::AddEntitySilently(Entity* entity)
{
	if (entities.insert(entity).second)
	{
		entitiesCache.clear();
		return true;
	}

	return false;
}

void Group::AddEntity(Entity * entity, ComponentID index, IComponent* component)
{
	if (AddEntitySilently(entity))
	{
		OnEntityAdded(this, entity, index, component);
	}
}

auto Group::AddEntity(Entity * entity) -> GroupChanged*
{
	return AddEntitySilently(entity) ? &OnEntityAdded : nullptr;
}

bool Group::RemoveEntitySilently(Entity * entity)
{
	if (entities.erase(entity))
	{
		entitiesCache.clear();
		return true;
	}

	return false;
}

void Group::RemoveEntity(Entity * entity, ComponentID index, IComponent* component)
{
	if (RemoveEntitySilently(entity))
	{
		OnEntityRemoved(this, entity, index, component);
	}
}

auto Group::RemoveEntity(Entity * entity) -> GroupChanged*
{
	return RemoveEntitySilently(entity) ? &OnEntityRemoved : nullptr;
}
}