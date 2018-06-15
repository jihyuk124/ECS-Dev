#pragma once
#include "MemoryLeakCheck.h"
#include "EntityPool.h"
#include "Entity.h"
#include "Position.h"
#include "Delegate.h"
using namespace ECS;
int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	EntityPool *entityPool = new EntityPool;
	Entity *entity = entityPool->CreateEntity();

	entity->Add<Position>(1.0f, 1.0f, 1.0f);
	//using del = Delegate<void(float x, float y, float z)>;


	delete entityPool;

	return 0;
}