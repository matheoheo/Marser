#include <iostream>
#include "parser//FileLoader.h"
#include "parser//FilePacker.h"
#include "encryption/EncryptionRegistry.h"

auto masterKey = matt::encryption::ByteVector{ std::byte{'K'}, std::byte{'O'} , std::byte{'T'} };
//178 81 107 28 24 205
void pack()
{
	matt::parser::FilePacker::PackerData data;
	data.encType = matt::encryption::EncryptionType::Xor;
	data.resultPath = "wynikowy.matt";
	data.masterKey = matt::encryption::ByteVector{ std::byte{'K'}, std::byte{'O'} , std::byte{'T'} };
	matt::parser::FilePacker::packFile("test.txt", data);
}

void unpack()
{
	auto result = matt::parser::FileLoader::loadFromFile("wynikowy.matt");
	matt::encryption::EncryptionRegistry registry;
	auto algo = registry.getAlgorithm(static_cast<matt::encryption::EncryptionType>(result.encryptionType),
		masterKey, std::as_bytes(std::span{ result.salt }));
	if (algo)
	{
		auto decoded = algo->decode(result.payload);
		std::string result(reinterpret_cast<const char*>(decoded.data()), decoded.size());
		std::cout << result << std::endl;
	}
}

int main()
{
	//pack();
	unpack();
	return 0;
}