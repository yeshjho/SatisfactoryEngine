#include "HashedString.hpp"



#ifdef _DEBUG
HashedString::HashedString(const xxhash::hash<64>::hash_type hash, std::string originalString)
	: mHash(hash), mOriginalString(std::move(originalString))
{
	originalStrings[hash] = mOriginalString;
}


HashedString hash_string(const std::string& str) noexcept
{
	return HashedString{ xxhash::xxh64(str.c_str(), str.size(), XXHASH_CX_XXH64_SEED), str };
}


HashedString operator ""_hs(const char* const str, const std::size_t len) noexcept
{
	return HashedString{ xxhash::xxh64(str, len, XXHASH_CX_XXH64_SEED), str };
}
#endif
