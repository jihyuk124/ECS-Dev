#pragma once
#include <stack>
#include <map>
#include <unordered_set>
#include <unordered_map>
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
	//Entity * CreateEntity(ComponentIDList indices);
	void Destroy();
	void RemoveEntity(Entity* entity);
	void RemoveAllEntity(Entity* entity);

	//auto GetEntities()->std::vector<Entity*>;
	//auto GetEntities(const Matcher matcher)->std::vector<Entity*>;
	//auto GetGroup(Matcher matcher)->Group*;

	/*void ClearGroups();
	void ResetCreationIndex();
	void ClearComponentPool(const ComponentID index);
	void ClearComponentPools();
	void Reset();

	auto GetEntityCount() const -> unsigned int;
	auto GetReusableEntitiesCount() const -> unsigned int;
	auto GetRetainedEntitiesCount() const -> unsigned int;*/

	//using PoolChanged = Delegate<void(EntityPool* pool, Entity* entity)>;
	//using GroupChanged = Delegate<void(EntityPool* pool, Group* group)>;

	/*PoolChanged OnEntityCreated;
	PoolChanged OnEntityWillBeDestroyed;
	PoolChanged OnEntityDestroyed;
	GroupChanged OnGroupCreated;
	GroupChanged OnGroupCleared;*/

private:
	//void UpdateGroupsComponentAddedOrRemoved(Entity* entity, ComponentID index, IComponent* component);
	//void UpdateGroupsComponentReplaced(Entity* entity, ComponentID index, IComponent* previousComponent, IComponent* newComponent);
	//void OnEntityReleased(Entity* entity);

private:
	unsigned int entityCounter;
	std::unordered_set<Entity*> entities;
	std::stack<Entity*> reusableEntities;
	/*std::vector<EntityPtr> mEntitiesCache;
	std::function<void(Entity*)> mOnEntityReleasedCache;*/

	std::map<ComponentID, std::stack<IComponent*>> componentPools;
	//std::map<ComponentID, std::vector<Group*>> mGroupsForIndex;

	//std::unordered_map<Matcher, Group*> Groups;

	
};
}
