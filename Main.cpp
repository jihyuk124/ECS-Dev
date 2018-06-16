#pragma once
#include "MemoryLeakCheck.h"
#include "EntityPool.h"
#include "Entity.h"
#include "Position.h"
#include "Delegate.h"
#include "Group.h"
#include "Matcher.h"
#include <iostream>

using namespace std;
using namespace ECS;
int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	EntityPool *pool = new EntityPool;

	pool->GetGroup(Matcher_AllOf(Position))->OnEntityAdded += [](Group* group, Entity* entity, ComponentID index, IComponent* component) {
		cout << "포지션 추가" << endl;
	};

	for (size_t i = 0; i < 10; i++)
	{
		Entity *entity = pool->CreateEntity();
		entity->Add<Position>(1.0f, 1.0f, 1.0f);
	}
	
	//using del = Delegate<void(float x, float y, float z)>;


	delete pool;

	return 0;
}