#pragma once
#include <map>

#include "ContainerMemoryAllocator.h"



namespace stf
{
template<typename Key, typename Value, typename Compare = std::less<Key>>
using Map = std::map<Key, Value, Compare, ContainerMemoryAllocator<std::pair<const Key, Value>>>;

template<typename Key, typename Value, typename Compare = std::less<Key>>
using MultiMap = std::multimap<Key, Value, Compare, ContainerMemoryAllocator<std::pair<const Key, Value>>>;
}
