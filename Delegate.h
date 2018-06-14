#pragma once
#include <list>
#include <mutex>
#include <memory>
#include <vector>

namespace ECS
{
template<typename>
class Delegate;
	
template<typename TReturnType, typename... TArgs>
class Delegate<TReturnType(TArgs...)>
{
	using functionType = std::function<TReturnType(TArgs...)>;

public:
	Delegate() = default;
	~Delegate() = default;

	Delegate(const Delegate&) = delete;
	const Delegate& operator =(const Delegate&) = delete;

	Delegate& Add(const functionType &function)
	{
		std::lock_guard<std::mutex> lock(this->mMutex);
		
		this->functionList.push_back(std::make_shared<functionType>(function));
		
		return *this;
	}

	Delegate& Remove(const functionType &function)
	{
		std::lock_guard<std::mutex> lock(this->mMutex);

		this->functionList.remove_if([&](std::shared_ptr<functionType> &functionPtr)
		{
			return Hash(function) == Hash(*functionPtr);
		});

		return *this;
	}
	inline typename void Invoke(TArgs... args)
	{
		std::lock_guard<std::mutex> lock(this->mMutex);

		for (const auto &functionPtr : functionList)
		{
			(*functionPtr)(args...);
		}
	}

	Delegate& Clear()
	{
		std::lock_guard<std::mutex> lock(this->mMutex);

		this->functionList.clear();

		return *this;
	}

	inline Delegate& operator +=(const functionType &function)
	{
		return Add(function);
	}

	inline Delegate& operator -=(const functionType &function)
	{
		return Remove(function);
	}

	inline typename void operator ()(TArgs... args)
	{
		Invoke(args...);
	}

private:
	std::mutex mMutex;
	std::list<std::shared_ptr<functionType>> functionList;

	inline constexpr size_t Hash(const functionType &function) const
	{
		return function.target_type().hash_code();
	}
};
}