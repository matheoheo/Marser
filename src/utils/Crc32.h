#pragma once
#include <cstdint>
#include <array>
#include <span>

namespace matt::utils
{
	class Crc32
	{
	public:
		static uint32_t compute(std::span<const std::byte> data);
	private:
		static constexpr std::array<uint32_t, 256> generateTable()
		{
			constexpr uint32_t crcStandard = 0xEDB88320;
			std::array<uint32_t, 256> result{};
			for (size_t i = 0; i < 256; ++i)
			{
				uint32_t entry = static_cast<uint32_t>(i);
				for (int bit = 0; bit < 8; ++bit)
				{
					if (entry & 1)
						entry = (entry >> 1) ^ crcStandard;
					else
						entry >>= 1;
				}
				result[i] = entry;
			}
			return result;
		}
	private:
	};
}