#include <iostream>
#include "io/MattFile.h"
#include "io/FileLoader.h"
#include <iomanip>
#include "parser/Tokenizer.h"


int main()
{
	/*
	//Test if the abstracted MattFile works

	//Pack plain .txt file
	matt::io::MattFile::saveFile("TestFile.txt", "ResultantFile.matt", matt::encryption::EncryptionType::Shift);

	//Read file as Text and print it to see if is decrypted
	auto str = matt::io::MattFile::loadAsText("ResultantFile.matt");

	std::cout << "Result is: " << std::endl << str << std::endl;
	*/
	auto content = matt::io::FileLoader::loadFromFileAsString("ParserTester.txt");
	matt::parser::Tokenizer tokenizer;
	auto tokens = tokenizer.tokenize(content);
	for (const auto& token : tokens)
	{
		std::cout << "Token type: " << static_cast<int>(token.type) << "    Token text: " << token.content << "    Token location: {" <<
			token.location.line << ',' << token.location.column << "}\n";
	}

	return 0;
}