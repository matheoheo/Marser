#include "XorEncryption.h"
#include "KeyDerivation.h"
#include <matt/logger/Logger.h>

matt::encryption::XorEncryption::XorEncryption(std::span<const std::byte> masterKey, std::span<const std::byte> saltKey)
    :IEncryptionAlgorithm(masterKey)
{
    mKey = KeyDerivation::xorSalt(masterKey, saltKey);
}

matt::encryption::ByteVector matt::encryption::XorEncryption::encode(std::span<const std::byte> bytes) const
{
    return performXor(bytes);
}

matt::encryption::ByteVector matt::encryption::XorEncryption::decode(std::span<const std::byte> bytes) const
{
    return performXor(bytes);
}

matt::encryption::EncryptionType matt::encryption::XorEncryption::getType() const
{
    return EncryptionType::Xor;
}

matt::encryption::ByteVector matt::encryption::XorEncryption::performXor(std::span<const std::byte> bytes) const
{
    if (mKey.empty())
    {
        MATT_ERROR("Key is empty, cannot continue.");
        return {};
    }

    ByteVector result;
    result.reserve(bytes.size());

    for (size_t i = 0; i < bytes.size(); ++i)
    {
        auto curr = bytes[i];
        size_t keyIndex = i % mKey.size();
        auto currKeyByte = mKey[keyIndex];
        result.emplace_back(curr ^ currKeyByte);
    }
    return result;
}
