#include <iostream>
#include "io/MattFile.h"
#include "io/FileLoader.h"
#include <iomanip>
#include "parser/Tokenizer.h"
#include "parser/Parser.h"

int main()
{
	auto rootValue = matt::parser::Parser::parseFile("testFile.txt");
	rootValue.debugPrint();
	return 0;
}