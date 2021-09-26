#include "MemoryManager.h"



namespace stf
{
using namespace literals;

#ifndef STF_INITIAL_MEMORY
#define STF_INITIAL_MEMORY 2_GB
#endif

MemoryManager MemoryManager::root{ STF_INITIAL_MEMORY };


MemoryManager::~MemoryManager()
{
	Disclaim();
}


MemoryManager::MemoryManager(MemoryManager&& other) noexcept
	: mAddress(other.mAddress), mSize(other.mSize),
	  mParent(other.mParent), mFirstChild(other.mFirstChild), mPrevSibling(other.mPrevSibling), mNextSibling(other.mNextSibling)
{
	MemoryManager* child = other.mFirstChild;
	while (child)
	{
		child->mParent = this;
		child = child->mNextSibling;
	}

	if (other.mParent && other.mParent->mFirstChild == &other)
	{
		other.mParent->mFirstChild = this;
	}

	if (other.mPrevSibling)
	{
		other.mPrevSibling->mNextSibling = this;
	}
	if (other.mNextSibling)
	{
		other.mNextSibling->mPrevSibling = this;
	}
}


MemoryManager MemoryManager::ClaimFragment(const size_t bytes)
{
	MemoryManager fragment{ claimFreeAddress(bytes), bytes };
	fragment.mParent = this;

	if (mFirstChild)
	{
		MemoryManager* lastChild = mFirstChild;
		while (lastChild->mNextSibling)
		{
			lastChild = lastChild->mNextSibling;
		}
		lastChild->mNextSibling = &fragment;
		fragment.mPrevSibling = lastChild;
	}
	else
	{
		mFirstChild = &fragment;
	}

	return fragment;
}


void MemoryManager::Disclaim()
{
	MemoryManager* child = mFirstChild;
	while (child)
	{
		MemoryManager* toKill = child;
		child = child->mNextSibling;
		toKill->Disclaim();
	}

	if (mParent && mParent->mFirstChild == this)
	{
		mParent->mFirstChild = mNextSibling;
	}

	if (mPrevSibling)
	{
		mPrevSibling->mNextSibling = mNextSibling;
	}
	if (mNextSibling)
	{
		mNextSibling->mPrevSibling = mPrevSibling;
	}

	operator delete(mAddress, mSize);

	mAddress = nullptr;
	mSize = 0;

	mParent = nullptr;
	mFirstChild = nullptr;
	mPrevSibling = nullptr;
	mNextSibling = nullptr;
}


MemoryManager::MemoryManager(const size_t bytes)
	: mAddress(operator new(bytes)), mSize(bytes)
{
}


MemoryManager::MemoryManager(void* const address, const size_t bytes)
	: mAddress(address), mSize(bytes)
{
}
}
