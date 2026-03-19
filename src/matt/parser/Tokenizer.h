#pragma once
#include "Token.h"
#include <vector>

namespace matt::parser
{
	using TokenVector = std::vector<Token>;
	class Tokenizer
	{
	public:
		Tokenizer();
		TokenVector tokenize(std::string_view content);
	private:
		bool isAtEnd() const;
		char peek() const;
		char peekNext() const;
		char advance();
		void scanToken(char c);
		void skipComment();
		void readString(const TokenLocation& loc);
		void readNumber(const TokenLocation& loc);
		void readIdentifier(const TokenLocation& loc);
		void addToken(TokenType type, const TokenLocation& loc);

	private:
		TokenVector mTokens;
		std::string_view mContent;
		size_t mLine;
		size_t mColumn;
		size_t mCurrent;
		size_t mStart;

		static constexpr char NullChar = '\0';
	};
}