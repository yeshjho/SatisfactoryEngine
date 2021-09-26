#pragma once
#include <filesystem>



namespace stf
{
enum class [[nodiscard]] EStorageType
{
	PLAYER,
	SHARED,
	TEMP
};

std::filesystem::path get_root_path(EStorageType storageType = EStorageType::PLAYER);
}
