#pragma once

namespace ECS
{
class IComponent
{
protected:
	IComponent() = default;
public:
	virtual ~IComponent() = default;
};
}