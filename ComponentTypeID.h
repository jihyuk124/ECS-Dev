#pragma once
#include "IComponent.h"
#include <vector>

namespace ECS
{
	typedef unsigned int ComponentID;
	typedef std::vector<ComponentID> ComponentIDList;

	class ComponentTypeID
	{
	public:
		template<typename T>
		static const ComponentID Get()
		{
			static_assert((std::is_base_of<IComponent, T>::value && !std::is_same<IComponent, T>::value),
				"Class type must be derived from IComponent");

			static ComponentID id = counter++;
			return id;
		}

		static unsigned int Count()
		{
			return counter;
		}

	private:
		static unsigned int counter;
	};
}