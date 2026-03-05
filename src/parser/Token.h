#pragma once
#include <cstdint>
#include <string>

namespace matt::parser
{
	enum class TokenType : uint8_t
	{
		LBrace, // {
		RBrace, // }
		LBracket, // [
		RBracket, // ]
		Colon, // :
		Identifier, // "hp"; "goblin" or else
		StringLiteral,
		NumberInt,
		NumberDouble,
		Boolean,
		Comma, // ,
		Comment, // #
		EndOfFile,
		Unknown,
	};
	struct TokenLocation
	{
		size_t line = 1;
		size_t column = 1;
	};
	struct Token
	{
		TokenType type;
		std::string_view content;
		TokenLocation location;
	};
}