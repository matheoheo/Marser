#pragma once
#include "IEncryptionAlgorithm.h"

namespace matt::encryption
{
	class XorEncryption : public IEncryptionAlgorithm
	{
	public:
		XorEncryption(std::span<const std::byte> masterKey, std::span<const std::byte> saltKey);
		virtual ByteVector encode(std::span<const std::byte> bytes) const override;
		virtual ByteVector decode(std::span<const std::byte> bytes) const override;
		virtual EncryptionType getType() const override;
	private:
		ByteVector performXor(std::span<const std::byte> bytes) const;
	};
}