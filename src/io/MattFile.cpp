#include "MattFile.h"
#include "FileLoader.h"
#include "FilePacker.h"
#include "encryption/KeyVault.h"
#include <fstream>

bool matt::io::MattFile::saveFile(const Path& fromFile, const Path& toFile, matt::encryption::EncryptionType encType, matt::encryption::KeyVault* keyVault)
{
	matt::encryption::KeyVault keys;
	auto packerData = matt::io::FilePacker::PackerData{};
	packerData.encType = encType;
	packerData.resultPath = toFile;
	packerData.masterKey = keyVault ? keyVault->getKeyForAlgorithm(encType) : keys.getKeyForAlgorithm(encType);

	return FilePacker::packFile(fromFile, packerData);
}

bool matt::io::MattFile::saveContent(std::string_view content, const Path& toFile, matt::encryption::EncryptionType encType, matt::encryption::KeyVault* keyVault)
{
	matt::encryption::KeyVault keys;
	auto packerData = matt::io::FilePacker::PackerData{};
	packerData.encType = encType;
	packerData.resultPath = toFile;
	packerData.masterKey = keyVault ? keyVault->getKeyForAlgorithm(encType) : keys.getKeyForAlgorithm(encType);

	return FilePacker::packContent(content, packerData);
}

matt::encryption::ByteVector matt::io::MattFile::loadFileRaw(const Path& fromFile, matt::encryption::KeyVault* keyVault)
{
	matt::encryption::KeyVault keys;
	auto package = FileLoader::loadFromFile(fromFile);
	auto encType = static_cast<matt::encryption::EncryptionType>(package.encryptionType);
	auto masterKey = keyVault ? keyVault->getKeyForAlgorithm(encType) : keys.getKeyForAlgorithm(encType);
	auto decodeAlgorithm = matt::encryption::EncryptionRegistry::getAlgorithm(encType, masterKey, std::as_bytes(std::span{package.salt}));
	if (decodeAlgorithm)
		return decodeAlgorithm->decode(package.payload);

	return package.payload;
}

std::string matt::io::MattFile::loadAsText(const Path& fromFile, matt::encryption::KeyVault* keyVault)
{
	auto bytes = loadFileRaw(fromFile, keyVault);
	return std::string(reinterpret_cast<const char*>(bytes.data()), bytes.size());
}

std::string matt::io::MattFile::loadPlain(const Path& fromFile)
{
	return FileLoader::loadFromFileAsString(fromFile);
}

bool matt::io::MattFile::isEncrypted(const Path& file)
{
	std::ifstream f(file, std::ios::binary);
	if (!f)
		return false;
	char header[constants::magicSize] = {};
	f.read(header, constants::magicSize);

	return std::memcmp(header, constants::magic, constants::magicSize) == 0;
}
