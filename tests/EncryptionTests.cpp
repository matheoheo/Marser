#include <gtest/gtest.h>
#include <span>
#include <type_traits>
#include <matt/random/Random.h>
#include <matt/logger/Logger.h>
#include "matt/encryption/XorEncryption.h"
#include "matt/encryption/ShiftEncryption.h"
#include "matt/encryption/IEncryptionAlgorithm.h"
#include "matt/encryption/KeyVault.h"
#include "matt/io/FilePacker.h"


template <typename EncAlgorithm>
requires std::derived_from<EncAlgorithm, matt::encryption::IEncryptionAlgorithm>
void testEncryptionAlgorithm()
{
	auto salt = matt::io::FilePacker::generateSalt();
	std::vector<std::byte> masterKey{};
	//Generate random master key
	for (size_t i = 0; i < 8; ++i)
		masterKey.emplace_back(static_cast<std::byte>(matt::random::get(0, 255)));

	//Generate random length of content string
	size_t stringLength = static_cast<size_t>(matt::random::get(90, 150));
	std::string testContent;
	testContent.resize(stringLength);

	//Generate string of random chars
	for (size_t i = 0; i < testContent.size(); ++i)
		testContent[i] = static_cast<unsigned char>(matt::random::get(32, 126));

	SCOPED_TRACE("testContent: " + testContent);

	auto bytesString = std::as_bytes(std::span{ testContent });
	matt::encryption::ByteVector bytesVersion(std::begin(bytesString), std::end(bytesString));

	EncAlgorithm alg(masterKey, salt);
	matt::encryption::ByteVector encoded = alg.encode(bytesVersion);
	matt::encryption::ByteVector decoded = alg.decode(encoded);
	std::string decodedString = std::string(reinterpret_cast<const char*>(decoded.data()), decoded.size());

	EXPECT_NE(encoded, bytesVersion);
	EXPECT_NE(decoded, encoded);
	EXPECT_EQ(testContent, decodedString);
}

TEST(EncryptionTest, XorEncryption)
{
	testEncryptionAlgorithm<matt::encryption::XorEncryption>();
}

TEST(EncryptionTest, ShiftEncryption)
{
	testEncryptionAlgorithm<matt::encryption::ShiftEncryption>();
}

//Test to add custom algorithm
namespace {
	inline constexpr uint8_t myCustomId = 130;
	class CustomAlgorithm : public matt::encryption::IEncryptionAlgorithm
	{
	public:
		CustomAlgorithm(std::span<const std::byte> masterKey)
			:IEncryptionAlgorithm(masterKey)
		{
			std::cout << "Master Key is:\n";
			for (auto byte : masterKey)
				std::cout << static_cast<unsigned char>(byte) << std::endl;
		}
		virtual matt::encryption::ByteVector encode(std::span<const std::byte> bytes) const override
		{
			std::cout << "Encoding bytes in custom algorithm\n";
			matt::encryption::ByteVector result;
			for (auto b : bytes)
				result.push_back(static_cast<std::byte>(static_cast<uint8_t>(b) + 1));
			return result;
		}
		virtual matt::encryption::ByteVector decode(std::span<const std::byte> bytes) const override
		{
			std::cout << "DECODING bytes in custom algorithm\n";
			matt::encryption::ByteVector result;
			for (auto b : bytes)
				result.push_back(static_cast<std::byte>(static_cast<uint8_t>(b) - 1));
			return result;
		}
		virtual matt::encryption::EncryptionType getType() const override
		{
			return myCustomId;
		}
	};
}

TEST(EncryptionTest, CustomEncryption)
{
	matt::encryption::ByteVector customKey = 
	{
		std::byte{'C'},
		std::byte{'U'},
		std::byte{'S'},
		std::byte{'T'},
		std::byte{'O'},
		std::byte{'M'},
	};

	matt::encryption::KeyVault keyVault;
	keyVault.addKey(myCustomId, customKey);
	EXPECT_EQ(customKey, keyVault.getKeyForAlgorithm(myCustomId));
	matt::encryption::EncryptionRegistry::registerAlgorithm(myCustomId, 
		[](std::span<const std::byte> key1, std::span<const std::byte> key2) {
			return std::make_unique<CustomAlgorithm>(key1);
		});

	constexpr std::string_view content = "Testing adding custom encryption algorithm. Greetings!";
	auto inputBytes = std::as_bytes(std::span{ content });
	auto inputData = matt::encryption::ByteVector(inputBytes.begin(), inputBytes.end());
	auto algorithm = matt::encryption::EncryptionRegistry::getAlgorithm(myCustomId, keyVault.getKeyForAlgorithm(myCustomId), {});
	ASSERT_NE(algorithm, nullptr) << "Registry failed to return Custom Algorithm.";

	auto encoded = algorithm->encode(inputData);
	EXPECT_FALSE(encoded.empty());
	auto decoded = algorithm->decode(encoded);
	EXPECT_FALSE(decoded.empty());

	EXPECT_EQ(inputData, decoded);
	EXPECT_NE(inputData, encoded);

	std::string decodedString = std::string(reinterpret_cast<const char*>(decoded.data()), decoded.size());
	EXPECT_EQ(decodedString, content);

	matt::encryption::EncryptionRegistry::unregister(myCustomId);
}