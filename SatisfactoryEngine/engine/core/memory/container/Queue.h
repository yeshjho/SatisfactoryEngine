#pragma once
#include <queue>

#include "Deque.h"
#include "Vector.h"



namespace stf
{
template<typename T>
using Queue = std::queue<T, Deque<T>>;

template<typename T, typename Compare = std::less<typename Vector<T>::value_type>>
using PriorityQueue = std::priority_queue<T, Vector<T>, Compare>;
}
