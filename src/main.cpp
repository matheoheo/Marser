#include <iostream>
#include "parser/Parser.h"
#include "io/MattFile.h"

//Full pipeline logic

int main()
{
	//Demonstrates packing a plain .txt file with encryption, then decrypting it back and parsing the content.
	const std::filesystem::path inputFile = "structureFile.txt";
	const std::filesystem::path resultFile = "structureResult.matt"; //matt extension is not required, could be anything
	matt::io::MattFile::saveFile(inputFile, resultFile, matt::encryption::Xor);
	matt::parser::Value parsedContent = matt::parser::Parser::parseFile(resultFile);
	std::string greet = parsedContent.get("engine.world.entities.1.dialogue.greeting").asString();
	std::cout << greet << std::endl;
	std::filesystem::remove(resultFile);
	return 0;
}