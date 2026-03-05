#include "ShiftEncryption.h"
#include "KeyDerivation.h"

matt::encryption::ShiftEncryption::ShiftEncryption(std::span<const std::byte> masterKey, std::span<const std::byte> saltKey)
	:IEncryptionAlgorithm(masterKey)
{
	mKey = KeyDerivation::xorSalt(masterKey, saltKey);
}

matt::encryption::ByteVector matt::encryption::ShiftEncryption::encode(std::span<const std::byte> bytes) const
{
	ByteVector result(bytes.size());
	for (size_t i = 0; i < bytes.size(); ++i)
	{
		uint8_t keyByte = static_cast<uint8_t>(mKey[i % mKey.size()]);
		uint8_t currByte = static_cast<uint8_t>(bytes[i]);
		result[i] = static_cast<std::byte>(keyByte + currByte);
	}
	return result;
}

matt::encryption::ByteVector matt::encryption::ShiftEncryption::decode(std::span<const std::byte> bytes) const
{
	ByteVector result(bytes.size());
	for (size_t i = 0; i < bytes.size(); ++i)
	{
		uint8_t keyByte = static_cast<uint8_t>(mKey[i % mKey.size()]);
		uint8_t currByte = static_cast<uint8_t>(bytes[i]);
		result[i] = static_cast<std::byte>(currByte - keyByte);
	}
	return result;
}

matt::encryption::EncryptionType matt::encryption::ShiftEncryption::getType() const
{
	return EncryptionType::Shift;
}
