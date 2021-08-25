#pragma once



namespace stf
{
enum class [[nodiscard]] EOSType
{
	WINDOWS,
	LINUX,
	MAC
};


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	constexpr EOSType OS_TYPE = EOSType::WINDOWS;
#elif __APPLE__
	constexpr EOSType OS_TYPE = EOSType::MAC;
#elif __linux__
	constexpr EOSType OS_TYPE = EOSType::LINUX;
#else
#error No Supported OS Detected
#endif
}
