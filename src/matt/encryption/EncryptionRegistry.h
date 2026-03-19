#pragma once
#include "IEncryptionAlgorithm.h"
#include "EncryptionTypes.h"
#include <memory>
#include <functional>

namespace matt::encryption
{
	class EncryptionRegistry
	{
		using Func = std::function<std::unique_ptr<IEncryptionAlgorithm>(std::span<const std::byte>, std::span<const std::byte>)>;
	public:
		EncryptionRegistry();
		static void registerAlgorithm(EncryptionType type, Func f);
		static void unregister(EncryptionType type);
		static std::unique_ptr<IEncryptionAlgorithm> getAlgorithm(EncryptionType type, std::span<const std::byte> masterKey, std::span<const std::byte> saltKey) ;
	private:
		static std::unordered_map<EncryptionType, Func>& getMap();
	};
}