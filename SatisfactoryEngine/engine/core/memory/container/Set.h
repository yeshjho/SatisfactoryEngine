#pragma once
#include <set>

#include "ContainerMemoryAllocator.h"



namespace stf
{
template<typename T, typename Compare = std::less<T>>
using Set = std::set<T, Compare, ContainerMemoryAllocator<T>>;

template<typename T, typename Compare = std::less<T>>
using MultiSet = std::multiset<T, Compare, ContainerMemoryAllocator<T>>;
}
