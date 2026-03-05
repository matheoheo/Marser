#include <iostream>
#include "io/MattFile.h"

int main()
{
	//Test if the abstracted MattFile works

	//Pack plain .txt file
	matt::io::MattFile::saveFile("TestFile.txt", "ResultantFile.matt", matt::encryption::EncryptionType::Shift);

	//Read file as Text and print it to see if is decrypted
	auto str = matt::io::MattFile::loadAsText("ResultantFile.matt");

	std::cout << "Result is: " << std::endl << str << std::endl;
	return 0;
}