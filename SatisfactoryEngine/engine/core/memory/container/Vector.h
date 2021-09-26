#pragma once
#include <vector>

#include "ContainerMemoryAllocator.h"



namespace stf
{
template<typename T>
using Vector = std::vector<T, ContainerMemoryAllocator<T>>;
}
