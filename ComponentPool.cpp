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
	//사용중인 컴포넌트들을 모두 제거하여 재사용 가능한 컴포넌트에서 모든 컴포넌트를 지우겠지만,
	//아직 지워지지 않은 컴포넌트들은 프로그램이 종료될 때 해제 되어야 하기 때문에 넣어둠.
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