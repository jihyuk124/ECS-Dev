#pragma once
namespace ECS
{
template<typename T>
class IPool
{
protected:
	IPool() = default;
public:
	virtual ~IPool() = default;

public:
	//template<typename TReturnType, typename ...TArgs>
	//TReturnType* Create(TArgs&& args) = 0;
	//virtual void Destroy() = 0;
};
}