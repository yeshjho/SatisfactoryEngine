#pragma once
#include <type_traits>

#include "../MemoryManager.h"



namespace stf
{
template<typename T>
class ContainerMemoryAllocator
{
	static_assert(!std::is_const_v<T>);
public:
	using value_type = T;
	using size_type = size_t;
	using difference_type = ptrdiff_t;

	using propagate_on_container_move_assignment = std::true_type;
	
	template<typename Other>
	constexpr ContainerMemoryAllocator(const ContainerMemoryAllocator<Other>&) noexcept {}

public:
	[[nodiscard]] T* allocate(size_t count);

	void deallocate(T* ptr, size_t count);

	[[nodiscard]] constexpr size_t max_size() const noexcept;
};
}

#include "ContainerMemoryAllocator.inl"
