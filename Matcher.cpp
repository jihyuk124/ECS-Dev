#include "Matcher.h"
#include <algorithm>
namespace ECS
{
auto Matcher::AllOf(const ComponentIDList indices) -> const Matcher
{
	auto matcher = Matcher();
	matcher.allOfIndices = DistinctIndices(indices);
	matcher.CalculateHash();

	return matcher;
}
auto Matcher::AllOf(const MatcherList matchers) -> const Matcher
{
	return Matcher::AllOf(MergeIndices(matchers));
}
auto Matcher::AnyOf(const ComponentIDList indices) -> const Matcher
{
	auto matcher = Matcher();
	matcher.anyOfIndices = DistinctIndices(indices);
	matcher.CalculateHash();

	return matcher;
}
auto Matcher::AnyOf(const MatcherList matchers) -> const Matcher
{
	return Matcher::AnyOf(MergeIndices(matchers)); 
}
auto Matcher::NoneOf(const ComponentIDList indices) -> const Matcher
{
	auto matcher = Matcher();
	matcher.noneOfIndices = DistinctIndices(indices);
	matcher.CalculateHash();

	return matcher;
}
auto Matcher::NoneOf(const MatcherList matchers) -> const Matcher
{
	return Matcher::NoneOf(MergeIndices(matchers));
}
bool Matcher::IsEmpty() const
{
	return (allOfIndices.empty() && anyOfIndices.empty() && noneOfIndices.empty());
}
bool Matcher::Matches(Entity *& entity)
{
	auto matchesAllOf = allOfIndices.empty() || entity->HasComponents(allOfIndices);
	auto matchesAnyOf = anyOfIndices.empty() || entity->HasAnyComponent(anyOfIndices);
	auto matchesNoneOf = noneOfIndices.empty() || !entity->HasAnyComponent(noneOfIndices);

	return matchesAllOf && matchesAnyOf && matchesNoneOf;
}
auto Matcher::GetIndices() -> const ComponentIDList
{
	if (indices.empty())
	{
		indices = MergeIndices();
	}

	return indices;
}
auto Matcher::GetAllOfIndices() const -> const ComponentIDList
{
	return allOfIndices;
}
auto Matcher::GetAnyOfIndices() const -> const ComponentIDList
{
	return anyOfIndices;
}
auto Matcher::GetNoneOfIndices() const -> const ComponentIDList
{
	return noneOfIndices;
}

auto Matcher::GetHashCode() const -> unsigned int
{
	return cachedHash;
}

bool Matcher::CompareIndices(const Matcher & matcher) const
{
	if (matcher.IsEmpty())
	{
		return false;
	}

	auto leftIndices = this->MergeIndices();
	auto rightIndices = matcher.MergeIndices();

	if (leftIndices.size() != rightIndices.size())
	{
		return false;
	}

	for (unsigned int i = 0, count = leftIndices.size(); i < count; ++i)
	{
		if (leftIndices[i] != rightIndices[i])
		{
			return false;
		}
	}

	return true;
}

bool Matcher::operator ==(const Matcher right) const
{
	return this->GetHashCode() == right.GetHashCode() && this->CompareIndices(right);
}

void Matcher::CalculateHash()
{
	unsigned int hash = typeid(Matcher).hash_code();

	hash = ApplyHash(hash, allOfIndices, 3, 53);
	hash = ApplyHash(hash, anyOfIndices, 307, 367);
	hash = ApplyHash(hash, noneOfIndices, 647, 683);

	cachedHash = hash;
}

auto Matcher::ApplyHash(unsigned int hash, const ComponentIDList indices, int i1, int i2) const -> unsigned int
{
	if (indices.size() > 0)
	{
		for (int i = 0, indicesLength = indices.size(); i < indicesLength; i++)
		{
			hash ^= indices[i] * i1;
		}

		hash ^= indices.size() * i2;
	}

	return hash;
}

auto Matcher::MergeIndices() const -> ComponentIDList
{
	auto indicesList = ComponentIDList();
	indicesList.reserve(allOfIndices.size() + anyOfIndices.size() + noneOfIndices.size());

	for (const auto &id : allOfIndices)
	{
		indicesList.push_back(id);
	}

	for (const auto &id : anyOfIndices)
	{
		indicesList.push_back(id);
	}

	for (const auto &id : noneOfIndices)
	{
		indicesList.push_back(id);
	}

	return DistinctIndices(indicesList);
}


auto Matcher::MergeIndices(MatcherList matchers) -> ComponentIDList
{
	unsigned int totalIndices = 0;

	for (auto &matcher : matchers)
	{
		totalIndices += matcher.GetIndices().size();
	}

	auto indices = ComponentIDList();
	indices.reserve(totalIndices);

	for (auto &matcher : matchers)
	{
		for (const auto &id : matcher.GetIndices())
		{
			indices.push_back(id);
		}
	}

	return indices;
}


auto Matcher::DistinctIndices(ComponentIDList indices) -> ComponentIDList
{
	std::sort(indices.begin(), indices.end());
	indices.erase(std::unique(indices.begin(), indices.end()), indices.end());

	return indices;
}

}