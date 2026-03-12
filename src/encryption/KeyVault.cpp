#include "KeyVault.h"

matt::encryption::KeyVault::KeyVault()
{
	initalize();
}

matt::encryption::KeyVault::~KeyVault()
{
	cleanup();
}

bool matt::encryption::KeyVault::addKey(EncryptionType type, const ByteVector& key)
{
	auto [it, emplaced] = mKeys.try_emplace(type, key);
	return emplaced;
}

bool matt::encryption::KeyVault::modifyKey(EncryptionType type, const ByteVector& key)
{
	auto it = mKeys.find(type);
	if (it == std::end(mKeys))
		return false;
	it->second = key;

	return true;
}

matt::encryption::ByteVector matt::encryption::KeyVault::getKeyForAlgorithm(EncryptionType type) const
{
	if (mKeys.contains(type))
		return mKeys.at(type);

	return {};
}

void matt::encryption::KeyVault::initalize()
{
	mKeys[Xor] = ByteVector{ std::byte{'M'}, std::byte{'A'}, std::byte{'T'}, std::byte{'H'}, std::byte{'E'}, std::byte{'O'},
		std::byte{'H'}, std::byte{'E'}, std::byte{'O'} };
	mKeys[Shift] = ByteVector{ std::byte{'M'}, std::byte{'A'}, std::byte{'R'}, std::byte{'S'}, std::byte{'E'}, std::byte{'R'} };
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
