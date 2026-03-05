#include "FileLoader.h"
#include <fstream>
#include <iostream>
#include <format>
#include <algorithm>
#include <bit>
#include <array>
#include "logger/Logger.h"
#include "FileHeader.h"
#include "utils/Crc32.h"
#include "Constants.h"

namespace matt::io
{
	EncryptedPackage FileLoader::loadFromFile(const std::filesystem::path& filePath)
	{
        MATT_INFO("Loading package from file:", filePath);

        std::ifstream sourceFile(filePath, std::ios::binary | std::ios::ate);
        if (!sourceFile)
        {
            MATT_ERROR("Failed to open file", filePath);
            return {};
        }

        auto dataSize = sourceFile.tellg();
        if (dataSize < sizeof(FileHeader))
        {
            MATT_ERROR("Data size is less than FileHeader");
            return {};
        }
        sourceFile.seekg(0, std::ios::beg);
        std::array<std::byte, sizeof(FileHeader)> rawHeader{};
        if (!sourceFile.read(reinterpret_cast<char*>(rawHeader.data()), rawHeader.size()))
        {
            MATT_ERROR("Failed to read header", filePath);
            return {};
        }
        FileHeader header = std::bit_cast<FileHeader>(rawHeader);

        if (std::memcmp(constants::magic, header.magic, constants::magicSize) != 0)
        {
            MATT_ERROR("Provided file lacks MATT signature", filePath);
            return {};
        }

        constexpr size_t maxPayloadSize = 100 * 1024 * 1024;
        if (header.payloadSize > maxPayloadSize)
        {
            MATT_ERROR("Payload size is too big:", header.payloadSize, "in file:", filePath);
            return {};
        }

        uint64_t actualPayloadSize = static_cast<uint64_t>(dataSize) - sizeof(FileHeader);
        if (actualPayloadSize != header.payloadSize)
        {
            MATT_ERROR("Expected payloadSize: ", header.payloadSize, " doesn't match the actual payloadSize: ", actualPayloadSize, "file: ", filePath);
            return {};
        }

        EncryptedPackage package;
        package.encryptionType = header.encryptionType;
        package.version = header.version;
        std::memcpy(package.salt, header.salt, constants::saltSize);
        package.payload.resize(static_cast<size_t>(actualPayloadSize));

        if (!sourceFile.read(reinterpret_cast<char*>(package.payload.data()), actualPayloadSize))
        {
            MATT_ERROR("Failed to read file's payload data", filePath);
            return {};
        }
        auto checksum = matt::utils::Crc32::compute(package.payload);
        if (header.checksum != checksum)
        {
            MATT_ERROR("Checksum value doesn't match, file is corrupted", filePath);
            return {};
        }
        return package;
	}

    std::string FileLoader::loadFromFileAsString(const std::filesystem::path& filePath)
    {
        MATT_INFO("Loading file as string: ", filePath);
        std::ifstream sourceFile(filePath, std::ios::binary | std::ios::ate);
        if (!sourceFile)
        {
            MATT_ERROR("Failed to open file: ", filePath);
            return {};
        }

        const size_t fileSize = static_cast<size_t>(sourceFile.tellg());
        sourceFile.seekg(0, std::ios::beg);
        std::string result(fileSize, '\0');
        if (!sourceFile.read(result.data(), fileSize))
        {
            MATT_ERROR("Failed to read file: ", filePath);
            return {};
        }
        return result;
    }
}

