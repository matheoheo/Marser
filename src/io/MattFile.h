#pragma once
#include <filesystem>
#include "encryption/EncryptionTypes.h"
#include "encryption/EncryptionRegistry.h"

namespace matt::io
{
	class MattFile
	{
	public:
		using Path = std::filesystem::path;
		//Packs, encrypts, and saves to disk in one go
		static bool saveFile(const Path& fromFile, const Path& toFile, matt::encryption::EncryptionType encType);
		//Loads from disk and decrypts into raw bytes
		static matt::encryption::ByteVector loadFileRaw(const Path& fromFile);
		//Loads, decrypts and converts to string
		static std::string loadAsText(const Path& fromFile);
	};
}