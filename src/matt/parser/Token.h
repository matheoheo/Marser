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

	inline std::string TokenTypeToStr(TokenType t)
	{
		switch (t)
		{
		case TokenType::LBrace: return "{";
		case TokenType::RBrace: return "}";
		case TokenType::Boolean: return "boolean";
		case TokenType::Colon: return ":";
		case TokenType::Comma: return ",";
		case TokenType::EndOfFile: return "<eof>";
		case TokenType::Identifier: return "Identifier";
		case TokenType::LBracket: return "[";
		case TokenType::NumberDouble: return "double";
		case TokenType::NumberInt: return "int";
		case TokenType::RBracket: return "]";
		case TokenType::StringLiteral: return "string";
		case TokenType::Unknown: return "unknown";
		default:
			return "default";
		}

		return "?";
	}
}