#include "Crc32.h"

uint32_t matt::utils::Crc32::compute(std::span<const std::byte> data)
{
	constexpr static auto lookupTable = generateTable();
	uint32_t accumulator = 0xFFFFFFFF;
	for (auto byte : data)
	{
		size_t index = (accumulator ^ std::to_integer<uint32_t>(byte)) & 0xFF;
		auto entry = lookupTable[index];
		accumulator = (accumulator >> 8) ^ entry;
	}
	accumulator ^= 0xFFFFFFFF;
	return accumulator;
}
