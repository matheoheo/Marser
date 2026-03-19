#pragma once
#include "Value.h"
#include "Tokenizer.h"
#include "matt/encryption/KeyVault.h"
#include <filesystem>
#include <charconv>
#include <span>
namespace matt::parser
{
	class Parser
	{
	public:
		Parser();
		static Value parseFile(const std::filesystem::path& file, matt::encryption::KeyVault* keyVault = nullptr);
		static Value parseContent(std::string_view content);
	private:
		Value internalParser(const TokenVector& tokens);
		Value parseValue();
		const Token& peek() const;
		const Token& previous() const;
		const Token& advance();
		bool isAtEnd() const;
		bool expect(TokenType type, std::string_view errorMessage);
		bool check(TokenType type) const;

		Value parseMap();
		Value parseList();
		void reportError(const Token& token, std::string_view errMessage);
		template <typename T>
		requires (std::same_as<T, int64_t> || std::same_as<T, double>)
		std::optional<T> parseToNumber(std::string_view str) const;
	private:
		std::span<const Token> mTokens;
		size_t mCurrent;
	};
	
	template<typename T>
	requires (std::same_as<T, int64_t> || std::same_as<T, double>)
	inline std::optional<T> Parser::parseToNumber(std::string_view str) const
	{
		T result = 0;
		auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), result);
		if (ec != std::errc())
			return std::nullopt;

		return result;
	}

}