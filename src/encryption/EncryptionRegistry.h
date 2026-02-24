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
		void registerAlgorithm(EncryptionType type, Func f);
		std::unique_ptr<IEncryptionAlgorithm> getAlgorithm(EncryptionType type, std::span<const std::byte> masterKey, std::span<const std::byte> saltKey) const;
	private:
		void populateRegistry();
	private:
		std::unordered_map<EncryptionType, Func> mRegistry;
	};
}