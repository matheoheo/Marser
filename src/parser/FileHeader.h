#pragma once
#include <cstdint>
#include "Constants.h"

namespace matt::parser
{
#pragma pack(push, 1)
	struct FileHeader
	{
		char magic[constants::magicSize]; //identifier ("MATT")
		uint8_t version;
		uint8_t encryptionType;
		uint8_t salt[constants::saltSize];
		
		uint64_t payloadSize;
		uint64_t originalSize;
		uint32_t checksum;
	};
#pragma pack(pop)

	static_assert(sizeof(FileHeader) == 32, "Sizeof FileHeader must be equal to 32 bytes!");
}