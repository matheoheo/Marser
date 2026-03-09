#include <gtest/gtest.h>
#include <span>
#include <type_traits>
#include "encryption/XorEncryption.h"
#include "encryption/ShiftEncryption.h"
#include "encryption/IEncryptionAlgorithm.h"
#include "io/FilePacker.h"
#include "utils/Random.h"

template <typename EncAlgorithm>
requires std::derived_from<EncAlgorithm, matt::encryption::IEncryptionAlgorithm>
void testEncryptionAlgorithm()
{
	auto salt = matt::io::FilePacker::generateSalt();
	std::vector<std::byte> masterKey{};
	//Generate random master key
	for (size_t i = 0; i < 8; ++i)
		masterKey.emplace_back(static_cast<std::byte>(Random::get<int>(0, 255)));

	//Generate random length of content string
	size_t stringLength = static_cast<size_t>(Random::get<int>(90, 150));
	std::string testContent;
	testContent.resize(stringLength);

	//Generate string of random chars
	for (size_t i = 0; i < testContent.size(); ++i)
		testContent[i] = static_cast<unsigned char>(Random::get<int>(32, 126));

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