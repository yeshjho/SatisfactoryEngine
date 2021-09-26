#pragma once
#include <forward_list>

#include "ContainerMemoryAllocator.h"



namespace stf
{
template<typename T>
using ForwardList = std::forward_list<T, ContainerMemoryAllocator<T>>;
}
