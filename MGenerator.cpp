#include "MGenerator.h"
#include <iostream>
#include <fstream>

bool matt::MGenerator::generateResultantFile(std::string_view sourcePath, std::string_view outputPath, EncryptionAlgorithm algorithm)
{
    MGenerator gen;
    auto sourceData = gen.readFile(sourcePath);
    if (sourceData.empty())
    {
        std::cout << "[MGenerator::generateResultantFile] Source data is empty, cannot pack file." << std::endl;
        return false;
    }


    return true;
}

matt::MGenerator::VecType matt::MGenerator::readFile(std::string_view sourcePath) const
{
    //Returns source's files data represented as vector of std::byte
    std::ifstream sourceFile(sourcePath.data(), std::ios::binary | std::ios::ate);
    if (!sourceFile)
    {
        std::cout << "[MGenerator::getSourceData] Failed to open source file: " << sourcePath << std::endl;
        return {};
    }

    auto dataSize = sourceFile.tellg();
    sourceFile.seekg(0, std::ios::beg); //moves cursor back to start.

    VecType result(dataSize);
    sourceFile.read(reinterpret_cast<char*>(result.data()), dataSize);

    return result;
}

matt::FileHeader matt::MGenerator::generateHeader(const VecType& data, EncryptionAlgorithm algorithm) const
{
    FileHeader header;
    header.encryption = algorithm;
    header.payloadSize = data.size();
    return FileHeader();
}
