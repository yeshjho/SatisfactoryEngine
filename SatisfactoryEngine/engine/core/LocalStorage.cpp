#include "LocalStorage.hpp"

#include "OSDetection.hpp"



namespace stf
{
std::filesystem::path get_root_path(const EStorageType storageType)
{
	static const std::filesystem::path userDir = []()
	{
		if constexpr (OS_TYPE == EOSType::WINDOWS)
		{
			char* pValue;
			size_t len;
			_dupenv_s(&pValue, &len, "APPDATA");
			return pValue;
		}
		else if constexpr (OS_TYPE == EOSType::MAC)
		{
			return "~/Library/Application Support/";
		}
		else if constexpr (OS_TYPE == EOSType::LINUX)
		{
			return "~/.local/aquaria/";
		}
	}();

	static const std::filesystem::path sharedDir = []()
	{
		if constexpr (OS_TYPE == EOSType::WINDOWS)
		{
			char* pValue;
			size_t len;
			_dupenv_s(&pValue, &len, "PROGRAMDATA");
			return pValue;
		}
		else if constexpr (OS_TYPE == EOSType::MAC)
		{
			return "/Library/Application Support/";
		}
		else if constexpr (OS_TYPE == EOSType::LINUX)
		{
			return "/var/games/";
		}
	}();

	static const std::filesystem::path tempDir = []()
	{
		if constexpr (OS_TYPE == EOSType::WINDOWS)
		{
			char* pValue;
			size_t len;
			_dupenv_s(&pValue, &len, "TEMP");
			return pValue;
		}
		else if constexpr (OS_TYPE == EOSType::MAC || OS_TYPE == EOSType::LINUX)
		{
			return "/tmp/";
		}
	}();

	switch (storageType)
	{
		case EStorageType::PLAYER:
			return userDir;

		case EStorageType::SHARED:
			return sharedDir;

		case EStorageType::TEMP:
			return tempDir;

		default:
			break;
	}
}
}
