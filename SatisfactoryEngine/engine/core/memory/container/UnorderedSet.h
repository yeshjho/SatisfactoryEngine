#pragma once
#include <unordered_set>

#include "ContainerMemoryAllocator.h"



namespace stf
{
template<typename T, typename Hash = std::hash<T>, typename Equal = std::equal_to<T>>
using UnorderedSet = std::unordered_set<T, Hash, Equal, ContainerMemoryAllocator<T>>;

template<typename T, typename Hash = std::hash<T>, typename Equal = std::equal_to<T>>
using UnorderedMultiSet = std::unordered_multiset<T, Hash, Equal, ContainerMemoryAllocator<T>>;
}
