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

		//Adds a key for provided encryption type
		//Encryption Key must be of type std::vector<std::byte>
		//returns true if successfull
		bool addKey(EncryptionType type, const ByteVector& key);

		//Allows to modify a key
		//Encryption Key must be of type std::vector<std::byte>
		bool modifyKey(EncryptionType type, const ByteVector& key);
		ByteVector getKeyForAlgorithm(EncryptionType type) const;
	private:
		void initalize();
		void cleanup();
	private:
		std::unordered_map<EncryptionType, ByteVector> mKeys;
	};
}