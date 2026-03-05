#pragma once
#include "IEncryptionAlgorithm.h"

namespace matt::encryption
{
	class ShiftEncryption : public IEncryptionAlgorithm
	{
	public:
		ShiftEncryption(std::span<const std::byte> masterKey, std::span<const std::byte> saltKey);
		// Inherited via IEncryptionAlgorithm
		virtual ByteVector encode(std::span<const std::byte> bytes) const override;
		virtual ByteVector decode(std::span<const std::byte> bytes) const override;
		virtual EncryptionType getType() const override;
	private:
		ByteVector performShift(std::span<const std::byte> bytes, bool encode) const;
	};
}