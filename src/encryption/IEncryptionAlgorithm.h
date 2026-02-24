#pragma once
#include <vector>
#include <span>
#include "EncryptionTypes.h"

namespace matt::encryption
{
	using ByteVector = std::vector<std::byte>;

	class IEncryptionAlgorithm
	{
	public:
		IEncryptionAlgorithm(std::span<const std::byte> masterKey);
		virtual ~IEncryptionAlgorithm() = default;
		virtual ByteVector encode(std::span<const std::byte> bytes) const = 0;
		virtual ByteVector decode(std::span<const std::byte> bytes) const = 0;
		virtual EncryptionType getType() const = 0;
	protected:
		ByteVector mKey;
	};
}
