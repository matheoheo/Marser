#include "KeyVault.h"
#include <atomic>

matt::encryption::KeyVault::KeyVault()
{
	initalize();
}

matt::encryption::KeyVault::~KeyVault()
{
	cleanup();
}

std::span<const std::byte> matt::encryption::KeyVault::getKeyForAlgorithm(EncryptionType type) const
{
	if (mKeys.contains(type))
		return mKeys.at(type);

	return {};
}

void matt::encryption::KeyVault::initalize()
{

}

void matt::encryption::KeyVault::cleanup()
{
	for (auto& [key, val] : mKeys)
	{
		std::fill(std::begin(val), std::end(val), std::byte{ 0 });
		//Must ensure that it is not optimzed away by compiler!!
		val.clear();
	}
	mKeys.clear();
}
