#pragma once
#include "IComponent.h"
namespace ECS
{
class Position : public IComponent
{
public:
	void Reset(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	float x;
	float y;
	float z;
};
}