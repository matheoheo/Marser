#include "XorEncryption.h"
#include "logger//Logger.h"
#include <numeric>

matt::encryption::XorEncryption::XorEncryption(std::span<const std::byte> masterKey, std::span<const std::byte> saltKey)
    :IEncryptionAlgorithm(masterKey)
{
    saltMasterKey(saltKey);
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

void matt::encryption::XorEncryption::saltMasterKey(std::span<const std::byte> saltKey)
{
    if (mKey.empty() || saltKey.empty())
    {
        MATT_ERROR("Master key or salt key is empty, cannot continue.");
        return;
    }

    auto masterSize = mKey.size();
    auto saltSize = saltKey.size();
    auto newSize = std::lcm(masterSize, saltSize);

    ByteVector newKey;
    newKey.reserve(newSize);
    for (size_t i = 0; i < newSize; ++i)
    {
        auto kByte = mKey[i % masterSize];
        auto sByte = saltKey[i % saltSize];
        newKey.push_back(kByte ^ static_cast<std::byte>(sByte));
    }
    mKey = std::move(newKey);
}
