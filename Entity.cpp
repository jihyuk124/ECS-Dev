#include "Entity.h"

namespace ECS
{
Entity::Entity(EntityID entityID)
{
	this->entityID = entityID;
}


Entity::~Entity()
{
}
bool Entity::IsActive() const
{
	return active;
}
bool Entity::HasComponent(const ComponentID index) const
{
	return (components.find(index) != components.end());
}
}