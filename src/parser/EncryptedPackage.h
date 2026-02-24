#pragma once
#include <cstdint>
#include <vector>
#include "Constants.h"

namespace matt::parser
{
	struct EncryptedPackage
	{
		using ByteVector = std::vector<std::byte>;
		uint8_t version;
		uint8_t encryptionType;
		ByteVector payload;
		uint8_t salt[constants::saltSize];

		EncryptedPackage()
			:version(0),
			encryptionType(0)
		{}
	};
}