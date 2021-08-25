#pragma once
#include <map>
#include <xxhash_cx/xxhash_cx.h>



namespace stf
{
#ifdef _DEBUG
class [[nodiscard]] HashedString final
{
public:
	HashedString(xxhash::hash<64>::hash_type hash, std::string originalString);

	constexpr operator xxhash::hash<64>::hash_type() const noexcept { return mHash; }

	[[nodiscard]] constexpr const std::string& GetDebugOnlyString() const noexcept { return mOriginalString; }
	[[nodiscard]] static constexpr const std::map<xxhash::hash<64>::hash_type, std::string>& GetDebugOnlyStrings() noexcept { return originalStrings; }

private:
	inline static std::map<xxhash::hash<64>::hash_type, std::string> originalStrings;

	xxhash::hash<64>::hash_type mHash;
	std::string mOriginalString;
};

HashedString hash_string(const std::string& str) noexcept;
HashedString operator""_hs(const char* str, std::size_t len) noexcept;
#else
using HashedString = xxhash::hash<64>::hash_type;

inline HashedString hash_string(const std::string& str) noexcept
{
	return xxhash::xxh64(str.c_str(), str.size(), XXHASH_CX_XXH64_SEED);
}

constexpr HashedString operator""_hs(const char* str, const std::size_t len) noexcept
{
	return xxhash::xxh64(str, len, XXHASH_CX_XXH64_SEED);
}
#endif
}
