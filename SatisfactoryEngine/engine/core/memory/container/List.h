#pragma once
#include <list>

#include "ContainerMemoryAllocator.h"



namespace stf
{
template<typename T>
using List = std::list<T, ContainerMemoryAllocator<T>>;
}
