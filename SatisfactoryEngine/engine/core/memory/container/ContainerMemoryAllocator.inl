#pragma once



namespace stf
{
template <typename T>
T* ContainerMemoryAllocator<T>::allocate(const size_t count)
{
	return static_cast<T*>(MemoryManager::root.Allocate(sizeof(T) * count));
}

template <typename T>
void ContainerMemoryAllocator<T>::deallocate(T* const ptr, const size_t count)
{
	MemoryManager::root.Deallocate(ptr, count);
}

template <typename T>
constexpr size_t ContainerMemoryAllocator<T>::max_size() const noexcept
{
	return MemoryManager::root.GetSize() / sizeof(T);
}
}
