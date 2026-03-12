#pragma once
#include <cstdint>

namespace matt::encryption
{
	//Library Defined types of encryption algorithms
	//This way od definining is much more extensible by potential users to define their own algorithms.
	//User should define their algorithms with values > 100 to lower the risk of potential type collisions in future.
	using EncryptionType = uint8_t;
	inline constexpr EncryptionType None = 1;
	inline constexpr EncryptionType Xor = 2;
	inline constexpr EncryptionType Shift = 3;
}
