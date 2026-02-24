#pragma once
#include <string_view>
#include <filesystem>
#include "EncryptedPackage.h"

namespace matt::parser
{
	class FileLoader
	{
	public:
		[[nodiscard]] static EncryptedPackage loadFromFile(const std::filesystem::path& filePath);
		[[nodiscard]] static std::string loadFromFileAsString(const std::filesystem::path& filePath);
	};
}
