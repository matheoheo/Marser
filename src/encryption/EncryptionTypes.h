#pragma once
#include <cstdint>

namespace matt::encryption
{
	enum class EncryptionType : uint8_t
	{
		None = 1,
		Xor = 8,
		Shift = 16
	};
}
