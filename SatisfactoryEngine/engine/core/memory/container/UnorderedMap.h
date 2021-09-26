#pragma once
#include <unordered_map>

#include "ContainerMemoryAllocator.h"



namespace stf
{
template<typename Key, typename Value, typename Hash = std::hash<Key>, typename Equal = std::equal_to<Key>>
using UnorderedMap = std::unordered_map<Key, Value, Hash, Equal, ContainerMemoryAllocator<std::pair<const Key, Value>>>;

template<typename Key, typename Value, typename Hash = std::hash<Key>, typename Equal = std::equal_to<Key>>
using UnorderedMultiMap = std::unordered_multimap<Key, Value, Hash, Equal, ContainerMemoryAllocator<std::pair<const Key, Value>>>;
}
