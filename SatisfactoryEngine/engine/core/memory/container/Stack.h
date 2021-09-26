#pragma once
#include <stack>

#include "Deque.h"



namespace stf
{
template<typename T>
using Stack = std::stack<T, Deque<T>>;
}
