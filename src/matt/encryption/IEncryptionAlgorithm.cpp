#include "IEncryptionAlgorithm.h"

matt::encryption::IEncryptionAlgorithm::IEncryptionAlgorithm(std::span<const std::byte> masterKey)
	:mKey(masterKey.begin(), masterKey.end())
{
}
