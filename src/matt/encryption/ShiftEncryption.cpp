#include "ShiftEncryption.h"
#include "KeyDerivation.h"
#include <matt/logger/Logger.h>

matt::encryption::ShiftEncryption::ShiftEncryption(std::span<const std::byte> masterKey, std::span<const std::byte> saltKey)
	:IEncryptionAlgorithm(masterKey)
{
	mKey = KeyDerivation::xorSalt(masterKey, saltKey);
}

matt::encryption::ByteVector matt::encryption::ShiftEncryption::encode(std::span<const std::byte> bytes) const
{
	return performShift(bytes, true);
}

matt::encryption::ByteVector matt::encryption::ShiftEncryption::decode(std::span<const std::byte> bytes) const
{
	return performShift(bytes, false);
}

matt::encryption::EncryptionType matt::encryption::ShiftEncryption::getType() const
{
	return Shift;
}

matt::encryption::ByteVector matt::encryption::ShiftEncryption::performShift(std::span<const std::byte> bytes, bool encode) const
{
	auto keySize = mKey.size();
	if (keySize == 0)
	{
		MATT_ERROR("Key size is 0, cannot continue");
		return {};
	}

	ByteVector result(bytes.size());
	for (size_t i = 0; i < bytes.size(); ++i)
	{
		uint8_t keyByte = std::to_integer<uint8_t>(mKey[i % keySize]);
		uint8_t currByte = std::to_integer<uint8_t>(bytes[i]);
		result[i] = encode ? std::byte(currByte + keyByte) : std::byte(currByte - keyByte);
	}
	return result;
}
