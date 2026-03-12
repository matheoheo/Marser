#include <gtest/gtest.h>
#include "parser/Value.h"
#include "io/MattFile.h"
#include "encryption/EncryptionTypes.h"
#include "parser/Parser.h"

TEST(IntegrationTest, FileLoaderPackerSymmetry)
{
	//Create test data as Value object
	matt::parser::Value root;
	root["metadata"]["app_name"] = "MParser";
	root["metadata"]["version"] = 1.0;
	root["tests"] = "GoogleTests";
	root["settings"]["resolution"]["width"] = 1920;
	root["settings"]["resolution"]["height"] = 1080;
	root["settings"]["resolution"]["fullscreen"] = true;
	matt::parser::Value::List dnsIps = { 
		matt::parser::Value(std::string("127.0.0.1")), 
		matt::parser::Value(std::string("192.168.0.1")), 
		matt::parser::Value(std::string("8.8.8.8")), 
		matt::parser::Value(std::string("8.8.4.4")) 
	};
	root["networks"]["dns"] = std::move(dnsIps);

	//get string based on the Value object
	auto rootStr = root.emitString();
	const std::string filename = "integration.matt";
	//save content with encryption
	ASSERT_TRUE(matt::io::MattFile::saveContent(rootStr, filename, matt::encryption::Xor));
	
	//Load content back to string (with decryption);
	std::string loadedContent = matt::io::MattFile::loadAsText(filename);
	auto valueContent = matt::parser::Parser::parseContent(loadedContent);

	EXPECT_EQ(valueContent["metadata"]["app_name"].asString(), "MParser");
	EXPECT_EQ(valueContent["tests"].asString(), "GoogleTests");
	EXPECT_EQ(valueContent["settings"]["resolution"]["width"].asInt(), 1920);
	EXPECT_EQ(valueContent["settings"]["resolution"]["fullscreen"].asBool(), true);

	EXPECT_EQ(valueContent.get("settings.resolution.height").asInt(), 1080);
	EXPECT_EQ(valueContent.get("networks.dns.0").asString(), "127.0.0.1");
	EXPECT_EQ(valueContent.get("networks.dns.1").asString(), "192.168.0.1");

	std::filesystem::remove(filename);
}