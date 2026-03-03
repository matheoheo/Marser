#include "FilePacker.h"
#include "logger/Logger.h"
#include "Constants.h"
#include "utils/Random.h"
#include "utils/Crc32.h"
#include "encryption/EncryptionTypes.h"
#include "encryption/EncryptionRegistry.h"
#include "FileLoader.h"
#include <fstream>

bool matt::parser::FilePacker::packContent(std::string_view content, const PackerData& data)
{
	if (content.empty())
	{
		MATT_ERROR("Content is empty, failed to pack:", data.resultPath);
		return false;
	}
	auto salt = generateSalt();
	auto cryptedData = cryptData(content, data, salt);
	auto header = generateHeader(content, data, salt, cryptedData);

	return writeToFile(data.resultPath, header, cryptedData);
}


bool matt::parser::FilePacker::packFile(const std::filesystem::path& sourcePath, const PackerData& data)
{
	auto content = FileLoader::loadFromFileAsString(sourcePath);
	return packContent(content, data);
}

matt::parser::FileHeader matt::parser::FilePacker::generateHeader(std::string_view content, const PackerData& data, std::span<const std::byte> saltKey, std::span<const std::byte> byteData)
{
	FileHeader result{};
	std::memcpy(result.magic, matt::parser::constants::magic, matt::parser::constants::magicSize);
	std::memcpy(result.salt, saltKey.data(), saltKey.size());
	result.version = matt::parser::constants::currentVersion;
	result.encryptionType = static_cast<uint8_t>(data.encType);
	result.originalSize = content.size();
	result.payloadSize = byteData.size();
	result.checksum = matt::utils::Crc32::compute(byteData);
	return result;
}


std::array<std::byte, matt::parser::constants::saltSize> matt::parser::FilePacker::generateSalt()
{
	auto size = constants::saltSize;
	std::array<std::byte, matt::parser::constants::saltSize> result;
	for (size_t i = 0; i < size; ++i)
		result[i] = static_cast<std::byte>(Random::get<int>(0, 255));

	return result;
}

matt::encryption::ByteVector matt::parser::FilePacker::cryptData(std::string_view content, const PackerData& data, std::span<const std::byte> saltKey)
{
	auto bytesContent = std::as_bytes(std::span{content});
	if (data.encType != encryption::EncryptionType::None)
	{
		matt::encryption::EncryptionRegistry registry;
		auto algo = registry.getAlgorithm(data.encType, data.masterKey, saltKey);
		if (algo)
			return algo->encode(bytesContent);
	}

	return matt::encryption::ByteVector(bytesContent.begin(), bytesContent.end());
}

bool matt::parser::FilePacker::writeToFile(const std::filesystem::path& resultPath, const FileHeader& header, std::span<const std::byte> cryptedData)
{
	std::ofstream resultFile(resultPath, std::ios::binary);
	if (!resultFile)
	{
		MATT_ERROR("Cannot open result file when packing", resultPath);
		return false;
	}
	if (!resultFile.write(reinterpret_cast<const char*>(&header), sizeof(header)) ||
		!resultFile.write(reinterpret_cast<const char*>(cryptedData.data()), cryptedData.size()))
	{
		MATT_ERROR("Failed to write to file: ", resultPath);
		return false;
	}

	return true;
}
