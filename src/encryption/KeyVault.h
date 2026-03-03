#pragma once
#include "EncryptionTypes.h"
#include "IEncryptionAlgorithm.h"
#include <unordered_map>
namespace matt::encryption
{
	class KeyVault
	{
	public:
		KeyVault();
		~KeyVault();

		std::span<const std::byte> getKeyForAlgorithm(EncryptionType type) const;
	private:
		void initalize();
		void cleanup();
	private:
		std::unordered_map<EncryptionType, ByteVector> mKeys;
	};
}