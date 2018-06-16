#pragma once
#include "Entity.h"
#include "Matcher.h"
#include <unordered_set>

namespace ECS
{
class Group
{
public:
	friend class EntityPool;
	Group(const Matcher& matcher);

	auto Count() const -> const unsigned int;
	auto GetEntities()->std::vector<Entity*>;
	auto GetSingleEntity() const->Entity*;
	bool ContainsEntity(Entity*& entity) const;
	auto GetMatcher() const->Matcher;
	//auto CreateObserver(const GroupEventType eventType)->GroupObserver*;

	using GroupChanged = Delegate<void(Group* group, Entity* entity, ComponentID index, IComponent* component)>;
	using GroupUpdated = Delegate<void(Group* group, Entity* entity, ComponentID index, IComponent* previousComponent, IComponent* newComponent)>;

	GroupChanged OnEntityAdded;
	GroupUpdated OnEntityUpdated;
	GroupChanged OnEntityRemoved;

private:
	auto HandleEntity(Entity* entity)->GroupChanged*;
	void HandleEntitySilently(Entity* entity);
	void HandleEntity(Entity* entity, ComponentID index, IComponent* component);
	void UpdateEntity(Entity* entity, ComponentID index, IComponent* previousComponent, IComponent* newComponent);
	void RemoveAllEventHandlers();


	bool AddEntitySilently(Entity* entity);
	void AddEntity(Entity* entity, ComponentID index, IComponent* component);
	auto AddEntity(Entity* entity)->GroupChanged*;
	bool RemoveEntitySilently(Entity* entity);
	void RemoveEntity(Entity* entity, ComponentID index, IComponent* component);
	auto RemoveEntity(Entity* entity)->GroupChanged*;

private:
	Matcher matcher;
	std::unordered_set<Entity*> entities;
	std::vector<Entity*> entitiesCache;
};

}