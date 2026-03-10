#include <iostream>
#include "parser/Parser.h"

//Parsing data example

int main()
{
    //Raw data (supports comments)
    std::string_view rawData = R"(
        # Connection Settings
        host: "127.0.0.1"
        port: 8080
        timeout: 5.5
    )";

    //You can use matt::parser::Parser::parseFile to read from file (automatically decrypts if its MATT File).
    matt::parser::Value parsedData = matt::parser::Parser::parseContent(rawData);

    std::cout << "Host is: " << parsedData["host"].asString() << std::endl;
    std::cout << "Port is: " << parsedData["port"].asInt() << std::endl;
    std::cout << "Timeout is: " << parsedData["timeout"].asDouble() << std::endl;

    /*
    * Output result is:
        Host is: 127.0.0.1
        Port is: 8080
        Timeout is: 5.5
    */

	return 0;
}