#pragma once
#include <utility>



namespace stf::literals
{
[[nodiscard]] consteval size_t operator""_KB(const size_t kb) noexcept
{
	return kb * 1024;
}

[[nodiscard]] consteval size_t operator""_MB(const size_t mb) noexcept
{
	return mb * 1024 * 1024;
}

[[nodiscard]] consteval size_t operator""_GB(const size_t gb) noexcept
{
	return gb * 1024 * 1024 * 1024;
}
}



namespace stf
{
class [[nodiscard]] MemoryManager
{
public:
	~MemoryManager();

	MemoryManager(MemoryManager&& other) noexcept;

	MemoryManager(const MemoryManager& other) = delete;
	MemoryManager& operator=(const MemoryManager& other) = delete;
	MemoryManager& operator=(MemoryManager&& other) = delete;

public:
	template<typename T, typename ...Args>
	[[nodiscard]] T* Construct(Args&&... args) noexcept(noexcept(new T{ std::forward<Args>(args)... }));

	template<typename T>
	void Destruct(T* ptr) noexcept(noexcept(ptr->~T()));

	[[nodiscard]] void* Allocate(size_t bytes);
	void Deallocate(void* ptr, size_t count);


	[[nodiscard]] constexpr void* GetAddress() const noexcept { return mAddress; }
	[[nodiscard]] constexpr size_t GetSize() const noexcept { return mSize; }


	MemoryManager ClaimFragment(size_t bytes);

	void Disclaim();

private:
	MemoryManager(size_t bytes);
	MemoryManager(void* address, size_t bytes);

	[[nodiscard]] void* claimFreeAddress(size_t bytes);



public:
	static MemoryManager root;

private:
	void* mAddress = nullptr;
	size_t mSize = 0;

	MemoryManager* mParent = nullptr;
	MemoryManager* mFirstChild = nullptr;
	MemoryManager* mPrevSibling = nullptr;
	MemoryManager* mNextSibling = nullptr;
};
}

#include "MemoryManager.inl"
