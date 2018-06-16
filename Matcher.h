#pragma once
#include "Entity.h"
namespace ECS
{
class Matcher;
using MatcherList = std::vector<Matcher>;
class Matcher
{
public:
	Matcher() = default;
	
	static auto AllOf(const ComponentIDList indices) -> const Matcher;
	static auto AllOf(const MatcherList matchers) -> const Matcher;
	static auto AnyOf(const ComponentIDList indices) -> const Matcher;
	static auto AnyOf(const MatcherList matchers) -> const Matcher;
	static auto NoneOf(const ComponentIDList indices) -> const Matcher;
	static auto NoneOf(const MatcherList matchers) -> const Matcher;

	bool IsEmpty() const;
	bool Matches(Entity*& entity);
	auto GetIndices() -> const ComponentIDList;
	auto GetAllOfIndices() const -> const ComponentIDList;
	auto GetAnyOfIndices() const -> const ComponentIDList;
	auto GetNoneOfIndices() const -> const ComponentIDList;

	auto GetHashCode() const -> unsigned int;
	bool CompareIndices(const Matcher& matcher) const;

	bool operator ==(const Matcher right) const;

private:
	void CalculateHash();

	auto ApplyHash(unsigned int hash, const ComponentIDList indices, int i1, int i2) const -> unsigned int;
	auto MergeIndices() const->ComponentIDList;
	static auto MergeIndices(MatcherList matchers)->ComponentIDList;
	static auto DistinctIndices(ComponentIDList indices)->ComponentIDList;

private:
	unsigned int cachedHash{ 0 };

	ComponentIDList indices;
	ComponentIDList allOfIndices;
	ComponentIDList anyOfIndices;
	ComponentIDList noneOfIndices;
};
}

namespace std
{
template <>
struct hash<ECS::Matcher>
{
	std::size_t operator()(const ECS::Matcher& matcher) const
	{
		return hash<unsigned int>()(matcher.GetHashCode());
	}
};
}

namespace
{
#define FUNC_1(MODIFIER, X) MODIFIER(X)
#define FUNC_2(MODIFIER, X, ...) MODIFIER(X), FUNC_1(MODIFIER, __VA_ARGS__)
#define FUNC_3(MODIFIER, X, ...) MODIFIER(X), FUNC_2(MODIFIER, __VA_ARGS__)
#define FUNC_4(MODIFIER, X, ...) MODIFIER(X), FUNC_3(MODIFIER, __VA_ARGS__)
#define FUNC_5(MODIFIER, X, ...) MODIFIER(X), FUNC_4(MODIFIER, __VA_ARGS__)
#define FUNC_6(MODIFIER, X, ...) MODIFIER(X), FUNC_5(MODIFIER, __VA_ARGS__)
#define GET_MACRO(_1, _2, _3, _4, _5, _6, NAME,...) NAME
#define FOR_EACH(MODIFIER,...) GET_MACRO(__VA_ARGS__, FUNC_6, FUNC_5, FUNC_4, FUNC_3, FUNC_2, FUNC_1)(MODIFIER, __VA_ARGS__)

#define COMPONENT_GET_TYPE_ID(COMPONENT_CLASS) ECS::ComponentTypeID::Get<COMPONENT_CLASS>()
#define Matcher_AllOf(...) (ECS::Matcher)ECS::Matcher::AllOf(std::vector<ECS::ComponentID>({ FOR_EACH(COMPONENT_GET_TYPE_ID, __VA_ARGS__) }))
#define Matcher_AnyOf(...) (ECS::Matcher)ECS::Matcher::AnyOf(std::vector<ECS::ComponentID>({ FOR_EACH(COMPONENT_GET_TYPE_ID, __VA_ARGS__) }))
#define Matcher_NoneOf(...) (ECS::Matcher)ECS::Matcher::NoneOf(std::vector<ECS::ComponentID>({ FOR_EACH(COMPONENT_GET_TYPE_ID, __VA_ARGS__) }))
}