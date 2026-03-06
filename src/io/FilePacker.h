#pragma once
#include "FileHeader.h"
#include <filesystem>
#include <array>
#include "encryption/EncryptionRegistry.h"

namespace matt::io
{
	class FilePacker
	{
	public:
		struct PackerData
		{
			std::filesystem::path resultPath;
			encryption::EncryptionType encType = encryption::EncryptionType::None;
			encryption::ByteVector masterKey;
		};
		static bool packContent(std::string_view content, const PackerData& data);
		static bool packFile(const std::filesystem::path& sourcePath, const PackerData& data);
	private:
		static FileHeader generateHeader(std::string_view content, const PackerData& data, std::span<const std::byte> saltKey, std::span<const std::byte> byteData);
		static std::array<std::byte, constants::saltSize> generateSalt();
		static matt::encryption::ByteVector cryptData(std::string_view content, const PackerData& data, std::span<const std::byte> saltKey);
		static bool writeToFile(const std::filesystem::path& resultPath, const FileHeader& header, std::span<const std::byte> bytesData);
	};
}