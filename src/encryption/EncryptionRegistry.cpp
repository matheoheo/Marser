#include "EncryptionRegistry.h"
#include "XorEncryption.h"
#include "ShiftEncryption.h"

matt::encryption::EncryptionRegistry::EncryptionRegistry()
{
}

void matt::encryption::EncryptionRegistry::registerAlgorithm(EncryptionType type, Func f)
{
	getMap().insert_or_assign(type, std::move(f));
}

void matt::encryption::EncryptionRegistry::unregister(EncryptionType type)
{
	auto& map = getMap();
	if (map.contains(type))
		map.erase(type);
}

std::unique_ptr<matt::encryption::IEncryptionAlgorithm> matt::encryption::EncryptionRegistry::getAlgorithm(EncryptionType type, std::span<const std::byte> masterKey, std::span<const std::byte> saltKey)
{
	auto& registry = getMap();
	auto it = registry.find(type);
	if (it == std::end(registry))
		return nullptr;

	return it->second(masterKey, saltKey);
}

std::unordered_map<matt::encryption::EncryptionType, matt::encryption::EncryptionRegistry::Func>& matt::encryption::EncryptionRegistry::getMap()
{
	static std::unordered_map<EncryptionType, Func> registry = []
	{
		static std::unordered_map<EncryptionType, Func> initalMap;
		initalMap[Xor] = [](std::span<const std::byte> masterKey, std::span<const std::byte> saltKey) {
			return std::make_unique<XorEncryption>(masterKey, saltKey);
		};
		initalMap[Shift] = [](std::span<const std::byte> masterKey, std::span<const std::byte> saltKey) {
			return std::make_unique<ShiftEncryption>(masterKey, saltKey);
		};

		return initalMap;
	}();
	
	return registry;
}
