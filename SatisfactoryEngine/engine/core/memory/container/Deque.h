#pragma once
#include <deque>

#include "ContainerMemoryAllocator.h"



namespace stf
{
template<typename T>
using Deque = std::deque<T, ContainerMemoryAllocator<T>>;
}
