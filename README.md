# Marser - C++ 20 Recursive Data Engine.

<p align="center">
  <img src="https://repository-images.githubusercontent.com/1146613328/af5fbb30-469a-4ee7-a130-3500fdd16d0d" width="80%" height="80%" alt="Marser Banner">
</p>

**Marser** is a lightweight, high-performance data serialization library built with **C++20**. 

It provides a type-safe, JSON-like structure for managing configuration data (or any other data you need), featuring "dotted.path" navigation, and built-in encryption modules.

---

# Encryption & Data Integrity.
Marser doesn't just store data; it secures it. The 'FilePacker' and 'FileLoader' modules implement a custom binary format, to ensure your configuration remains private and uncorrupted.

1. **Ciphers** Built-in **Xor** and **Shift** encryption strategies.
2. **Integrity Verification** Every file includes **Checksum CRC** and **Magic Header**
3. **Corruption Detection** The 'FileLoader' validates the checksum and header before parsing, preventing crashes from corrupted files.


# The Parsing Engine
Marser utilizies a **Top Down Parser** coupled with a custom **Tokenizer**

1. **Tokenization** The source text is broken down into vector of identified symbols.
2. **Recursive Logic** Recursion is used to walk the token stream.
3. **Error Handling** The parser validates syntax as it goes, ensuring that invalid symbols are caught early.

# Modern C++ 20 implementation
Marser relies on modern C++ to ensure zero unnecessary allocations, type safety and good performance.

1. **Value** is implemented as a **std::variant**. This allows to handle multiple data types within a single object while preventing 'type-confusion' bugs.
2. **Minimized heap allocations** Objects like **std::string_view** or **std::span** are used extensively throughout the code to reference existing memory, rather than copying it.
3. **Dotted.Path** takes advantage of functionalities like **std::ranges::views::split** and **std::from_chars** to navigate the deep data structure without any copies.

# Usage Example
You can use **Marser** to read configuration files, but not only. The library provides functionalities, such as saving files with encryption mechanism, loading them back and decrypting. You might also create your own Value map, pack it and save to file.

1. Basic Value creation and dotted.path access
```C++
#include <iostream>
#include "parser/Value.h"

//Basic Value object creation 
//Dotted.path access

int main()
{
	//Auto Vivification - you can create maps and lists as long as you assign values
	matt::parser::Value root;
	root["server"]["ip"] = matt::parser::Value(std::string("127.0.0.1"));
	root["server"]["ports"] = matt::parser::Value::List{ matt::parser::Value{80}, matt::parser::Value{443}, matt::parser::Value{8080} };
	
	//Example of using the 'dotted.path' navigation
	auto connectionIp = root.get("server.ip").asString();
	auto primaryPort = root.get("server.ports.0").asInt();

	std::cout << "Connection Ip is: " << connectionIp << '\n';
	std::cout << "Primary port is: " << primaryPort << '\n';
	/*
		Output result:
		Connection Ip is: 127.0.0.1
		Primary port is: 80
	*/
	return 0;
}
```

2. Using **MattFile class** to save and load content.
 ```C++
 #include <iostream>
#include "parser/Value.h"
#include "io/MattFile.h"

//Saving with encryption
//Loading files

int main()
{
	//Auto Vivification - you can create maps and lists as long as you assign values
	matt::parser::Value root;
	root["server"]["ip"] = matt::parser::Value(std::string("127.0.0.1"));
	root["server"]["ports"] = matt::parser::Value::List{ matt::parser::Value{80}, matt::parser::Value{443}, matt::parser::Value{8080} };
	
	//Get string object of value
	std::string rootString = root.emitString();

	//Save content to file with XOR Encryption
	//Key is saved inside 'KeyVault' class.
	bool contentSaved = matt::io::MattFile::saveContent(rootString, "rootExample1.matt", matt::encryption::EncryptionType::Xor);
	std::cout << "Content saved status: " << std::boolalpha << contentSaved << '\n';
	
	//Load content back (MattFile manages decryption automatically)
	std::string rootContent = matt::io::MattFile::loadAsText("rootExample1.matt");
	bool rootCotentEqualsRootString = rootString == rootContent;

	std::cout << "Loaded content is the same: " << rootCotentEqualsRootString << '\n';

	/*
		Output result:
		Content saved status: true
		[INFO]  FileLoader.cpp:loadFromFile():17 Loading package from file: "rootExample1.matt"
		Loaded content is the same: true
	*/

	return 0;
}
```
3. Using **Parser** functionality to get an easy access to all the data.
```C++
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
```

# Extensibility: Adding your own encryption
**Marser** is designed to be modular. You can add a custom encryption algorithm by following two simple steps:
1. Inherit from **IEncryptionAlgorithm**: Create a new class that overrides the virtual methods.
2. Register your Type: Add your new type to **EncryptionType** enum.

Once implemented, the **FilePacker** and **FileLoader** will automatically be able to use your new algorithm, without any change to core logic.

# Build & Testing
**Marser** uses **CMAKE (>= 3.15)** for building, and integraes **GoogleTests** for running unit tests.
# Requirements
  - Compiler supporting C++20
  - Installed [CMake](https://cmake.org/)
  - Installed [Git](https://git-scm.com/downloads)
# Build Instructions (for windows)
  1. Open command terminal (**cmd**)
  2. Navigate to where you want to clone the project (**cd C:/Projects**)
  3. Clone the repository (**git clone https://github.com/matheoheo/Marser.git**)
  4. Enter the project folder (**cd Marser**)
  5. Create build directory and navigate there (**mkdir build & cd build**)
  6. Configure the project (**cmake ..**)
  7. Compile the project (**cmake --build .**)

Now you can run tests by typing the command **ctest** from **build** folder level, or you can navigate to **/bin/Debug** with **cd bin/Debug** and run **MarserTests.exe**
