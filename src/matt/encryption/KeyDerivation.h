#pragma once
#include "IEncryptionAlgorithm.h"

namespace matt::encryption
{
	class KeyDerivation
	{
	public:
		[[nodiscard]] static ByteVector xorSalt(std::span<const std::byte> masterKey, std::span<const std::byte> saltKey);
	};
}