#pragma once



namespace stf
{
template <typename T, typename ... Args>
T* MemoryManager::Construct(Args&&... args) noexcept(noexcept(new T{ std::forward<Args>(args)... }))
{
	return new(claimFreeAddress(sizeof(T))) T{ std::forward<Args>(args)... };
}

template <typename T>
void MemoryManager::Destruct(T* ptr) noexcept(noexcept(ptr->~T()))
{
	ptr->~T();
}
}
