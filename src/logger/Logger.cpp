#include "Logger.h"
#include <filesystem>

std::string_view matt::Logger::extractFilename(std::string_view fullName)
{
	size_t lastSlash = fullName.find_last_of("\\/");
	if (lastSlash == std::string_view::npos)
		return fullName;

	return fullName.substr(lastSlash + 1);
}

void matt::Logger::printLocation(std::source_location loc)
{
	auto name = extractFilename(loc.file_name());
	std::cout << name << ":" << loc.function_name() << "():" << loc.line() << " ";
}
