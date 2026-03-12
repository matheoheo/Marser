#include "KeyDerivation.h"
#include <numeric>
#include <matt/logger/Logger.h>


matt::encryption::ByteVector matt::encryption::KeyDerivation::xorSalt(std::span<const std::byte> masterKey, std::span<const std::byte> saltKey)
{
    if (masterKey.empty() || saltKey.empty())
    {
        MATT_ERROR("Master key or salt key is empty, cannot salt key.");
        return {};
    }

    auto masterSize = masterKey.size();
    auto saltSize = saltKey.size();
    auto newSize = std::lcm(masterSize, saltSize);

    ByteVector newKey(newSize);
    for (size_t i = 0; i < newSize; ++i)
    {
        auto kByte = masterKey[i % masterSize];
        auto sByte = saltKey[i % saltSize];
        newKey[i] = kByte ^ sByte;
    }
    return newKey;
}
