#pragma once
#include <cstdint>

namespace matt::parser::constants
{
	inline constexpr size_t magicSize = 4;
	inline constexpr char magic[magicSize] = { 'M', 'A', 'T', 'T' };
	inline constexpr uint8_t currentVersion = 1;
	inline constexpr size_t saltSize = 6;
}