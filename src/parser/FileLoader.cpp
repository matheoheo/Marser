#include "FileLoader.h"
#include <fstream>
#include <iostream>
#include <format>
#include <algorithm>
#include "logger/Logger.h"
#include "FileHeader.h"
#include "Constants.h"

namespace matt::parser
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
        FileHeader header;
        if (!sourceFile.read(reinterpret_cast<char*>(&header), sizeof(FileHeader)))
        {
            MATT_ERROR("Failed to read header", filePath);
            return {};
        }

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
        return package;
	}
    std::string FileLoader::loadFromFileAsString(const std::filesystem::path& filePath)
    {
        MATT_INFO("Loading file as string: ", filePath);
        std::ifstream sourceFile(filePath, std::ios::binary);
        if (!sourceFile)
        {
            MATT_ERROR("Failed to open file: ", filePath);
            return {};
        }

        return std::string((std::istreambuf_iterator<char>(sourceFile)),std::istreambuf_iterator<char>()) ;
    }
}

