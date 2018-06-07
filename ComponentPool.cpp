#include "ComponentPool.h"
#include "IComponent.h"

namespace ECS
{
ComponentPool::ComponentPool()
{
}

ComponentPool::~ComponentPool()
{
	Destroy();
}

void ECS::ComponentPool::Destroy()
{
	//������� ������Ʈ���� ��� �����Ͽ� ���� ������ ������Ʈ���� ��� ������Ʈ�� ���������,
	//���� �������� ���� ������Ʈ���� ���α׷��� ����� �� ���� �Ǿ�� �ϱ� ������ �־��.
	for (auto &pair : components)
	{
		auto &componentGroup = pair.second;
		while (false == componentGroup.empty())
		{
			delete componentGroup.top();
			componentGroup.pop();
		}
	}
	components.clear();

	for (auto &pair : reusableComponents)
	{
		auto &componentGroup = pair.second;
		while (false == componentGroup.empty())
		{
			delete componentGroup.top();
			componentGroup.pop();
		}
	}
	reusableComponents.clear();
}
}