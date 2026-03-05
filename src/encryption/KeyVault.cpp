#include "KeyVault.h"

matt::encryption::KeyVault::KeyVault()
{
	initalize();
}

matt::encryption::KeyVault::~KeyVault()
{
	cleanup();
}

matt::encryption::ByteVector matt::encryption::KeyVault::getKeyForAlgorithm(EncryptionType type) const
{
	if (mKeys.contains(type))
		return mKeys.at(type);

	return {};
}

void matt::encryption::KeyVault::initalize()
{
	mKeys[EncryptionType::Xor] = ByteVector{ std::byte{'M'}, std::byte{'A'}, std::byte{'T'}, std::byte{'H'}, std::byte{'E'}, std::byte{'O'},
		std::byte{'H'}, std::byte{'E'}, std::byte{'O'} };
}

void matt::encryption::KeyVault::cleanup()
{
	for (auto& [key, val] : mKeys)
	{
		//Volatile is required so the compilator will not optimize away
		for (volatile auto& k : val)
			k = std::byte{ 0 };
		val.clear();
	}
	mKeys.clear();
}
