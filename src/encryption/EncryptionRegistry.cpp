#include "EncryptionRegistry.h"
#include "XorEncryption.h"

matt::encryption::EncryptionRegistry::EncryptionRegistry()
{
	populateRegistry();
}

void matt::encryption::EncryptionRegistry::registerAlgorithm(EncryptionType type, Func f)
{
	mRegistry.try_emplace(type, std::move(f));
}

std::unique_ptr<matt::encryption::IEncryptionAlgorithm> matt::encryption::EncryptionRegistry::getAlgorithm(EncryptionType type, std::span<const std::byte> masterKey, std::span<const std::byte> saltKey) const
{
	auto it = mRegistry.find(type);
	if (it == std::end(mRegistry))
		return nullptr;

	return it->second(masterKey, saltKey);
}

void matt::encryption::EncryptionRegistry::populateRegistry()
{
	registerAlgorithm(EncryptionType::Xor, [](std::span<const std::byte> masterKey, std::span<const std::byte> saltKey)
		{
			return std::make_unique<XorEncryption>(masterKey, saltKey);
		});
}
