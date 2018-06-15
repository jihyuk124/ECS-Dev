#include "Entity.h"

namespace ECS
{
Entity::Entity(std::map<ComponentID, std::stack<IComponent*>>* componentPools)
{
	this->componentPools = componentPools;
}


Entity::~Entity()
{
}
bool Entity::IsActive() const
{
	return active;
}


void Entity::RemoveAllComponents()
{
	{
		auto componentsIdTemp = std::vector<ComponentID>(components.size());

		for (const auto &pair : components)
		{
			componentsIdTemp.push_back(pair.first);
		}

		while (!components.empty())
		{
			Replace(componentsIdTemp.back(), nullptr);
			componentsIdTemp.pop_back();
		}
	}
}

bool Entity::operator==(const Entity*& right) const
{
	return this->entityID == right->entityID;
}

Entity* Entity::AddComponent(ComponentID index, IComponent* component)
{
	if (!active)
	{
		throw std::runtime_error("Error, cannot add component to entity, entity has already been destroyed.");
	}

	if (HasComponent(index))
	{
		throw std::runtime_error("Error, cannot add component to entity, component already exists");
	}

	components[index] = component;
	OnComponentAdded(this, index, component);

	return this;
}


Entity * Entity::RemoveComponent(ComponentID index)
{
	if (!active)
	{
		throw std::runtime_error("Error, cannot remove component to entity, entity has already been destroyed.");
	}

	if (!HasComponent(index))
	{
		throw std::runtime_error("Error, cannot remove component to entity, component not exists");
	}

	Replace(index, nullptr);

	return this;
}

Entity * Entity::ReplaceComponent(const ComponentID index, IComponent * component)
{
	if (!active)
	{
		throw std::runtime_error("Error, cannot replace component to entity, entity has already been destroyed.");
	}

	if (HasComponent(index))
	{
		Replace(index, component);
	}
	else if (component != nullptr)
	{
		AddComponent(index, component);
	}

	return this;
}

IComponent * Entity::GetComponent(const ComponentID index) const
{
	if (!HasComponent(index))
	{
		throw std::runtime_error("Error, cannot get component from entity, component not exists");
	}
	return components.at(index);
}

bool Entity::HasComponent(const ComponentID index) const
{
	return (components.find(index) != components.end());
}

void Entity::Destroy()
{
	RemoveAllComponents();
	OnComponentAdded.Clear();
	OnComponentReplaced.Clear();
	OnComponentRemoved.Clear();
	active = false;
}
std::stack<IComponent *>* Entity::GetComponentPool(const ComponentID index) const
{
	return &((*componentPools)[index]);
}
void Entity::Replace(const ComponentID index, IComponent * replacement)
{
	auto previousComponent = GetComponent(index);

	if (previousComponent == replacement)
	{
		OnComponentReplaced(this, index, previousComponent, replacement);
	}
	else
	{
		GetComponentPool(index)->push(previousComponent);

		if (replacement == nullptr)
		{
			components.erase(index);
			OnComponentRemoved(this, index, previousComponent);
		}
		else
		{
			components[index] = replacement;
			OnComponentReplaced(this, index, previousComponent, replacement);
		}
	}
}
}