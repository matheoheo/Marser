#include "MattFile.h"
#include "FileLoader.h"
#include "FilePacker.h"
#include "encryption/KeyVault.h"
bool matt::io::MattFile::saveFile(const Path& fromFile, const Path& toFile, matt::encryption::EncryptionType encType)
{
	matt::encryption::KeyVault keys;
	auto packerData = matt::io::FilePacker::PackerData{};
	packerData.encType = encType;
	packerData.resultPath = toFile;
	packerData.masterKey = keys.getKeyForAlgorithm(encType);

	return FilePacker::packFile(fromFile, packerData);
}

matt::encryption::ByteVector matt::io::MattFile::loadFileRaw(const Path& fromFile)
{
	matt::encryption::KeyVault keys;
	matt::encryption::EncryptionRegistry algorithms;
	auto package = FileLoader::loadFromFile(fromFile);
	auto encType = static_cast<matt::encryption::EncryptionType>(package.encryptionType);
	auto masterKey = keys.getKeyForAlgorithm(encType);
	auto decodeAlgorithm = algorithms.getAlgorithm(encType, masterKey, std::as_bytes(std::span{package.salt}));
	if (decodeAlgorithm)
		return decodeAlgorithm->decode(package.payload);

	return package.payload;
}

std::string matt::io::MattFile::loadAsText(const Path& fromFile)
{
	auto bytes = loadFileRaw(fromFile);
	return std::string(reinterpret_cast<const char*>(bytes.data()), bytes.size());
}
