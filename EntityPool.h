#pragma once
#include <stack>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include "Entity.h"
#include "Group.h"

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
	//Entity * CreateEntity(ComponentIDList indices);
	void Destroy();
	void RemoveEntity(Entity* entity);

	bool HasEntity(Entity*& entity) const;
	auto GetEntities()->std::vector<Entity*>;
	auto GetEntities(const Matcher matcher)->std::vector<Entity*>;
	auto GetGroup(Matcher matcher)->Group*;

	auto GetEntityCount() const -> unsigned int;
	auto GetReusableEntitiesCount() const -> unsigned int;

	using PoolChanged = Delegate<void(EntityPool* pool, Entity* entity)>;
	using GroupChanged = Delegate<void(EntityPool* pool, Group* group)>;

	PoolChanged OnEntityCreated;
	PoolChanged OnEntityWillBeDestroyed;
	PoolChanged OnEntityDestroyed;
	GroupChanged OnGroupCreated;
	GroupChanged OnGroupCleared;

private:
	void UpdateGroupsComponentAddedOrRemoved(Entity* entity, ComponentID index, IComponent* component);
	void UpdateGroupsComponentReplaced(Entity* entity, ComponentID index, IComponent* previousComponent, IComponent* newComponent);
	void OnEntityReleased(Entity* entity);

private:
	unsigned int entityCounter;
	std::unordered_set<Entity*> entities;
	std::stack<Entity*> reusableEntities;
	std::vector<Entity*> entitiesCache;
	std::function<void(Entity*)> OnEntityReleasedCache;

	std::map<ComponentID, std::stack<IComponent*>> componentPools;

	std::map<ComponentID, std::vector<Group*>> groupsForIndex;
	std::unordered_map<Matcher, Group*> groups;

	
};
}
