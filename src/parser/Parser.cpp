#include "Parser.h"
#include "io/MattFile.h"
#include "logger/Logger.h"
#include <iostream>

matt::parser::Parser::Parser()
	:mCurrent(0)
{
}

matt::parser::Value matt::parser::Parser::parseFile(const std::filesystem::path& file)
{
	std::string content;
	if (matt::io::MattFile::isEncrypted(file))
		content = matt::io::MattFile::loadAsText(file);
	else
		content = matt::io::MattFile::loadPlain(file);

	return parseContent(content);
}

matt::parser::Value matt::parser::Parser::parseContent(std::string_view content)
{
	Tokenizer tokenizer;
	auto tokens = tokenizer.tokenize(content);
	Parser parser;
	return parser.internalParser(tokens);
}

matt::parser::Value matt::parser::Parser::internalParser(const TokenVector& tokens)
{
	mTokens = tokens;
	mCurrent = 0;
	Value root(Value::Map{});
	while (!isAtEnd() && !check(TokenType::EndOfFile))
	{
		if (check(TokenType::Identifier))
		{
			std::string key(advance().content);
			if (check(TokenType::LBrace))
				root[key] = parseMap();
			else if (check(TokenType::LBracket))
				root[key] = parseList();
			else
			{
				if (expect(TokenType::Colon, "Expected ':' after key"))
					root[key] = parseValue();
			}
		}
		else
			advance();
		
	}
	return root;
}

matt::parser::Value matt::parser::Parser::parseValue()
{
	if (isAtEnd())
		return Value{};

	auto& token = peek();
	switch (token.type)
	{
	case TokenType::Identifier:
	{
		return Value(advance().content);
	}
	case TokenType::NumberInt:
	{
		auto val = parseToNumber<int64_t>(advance().content);
		return val ? Value(*val) : Value();
	}
	case TokenType::NumberDouble:
	{
		auto val = parseToNumber<double>(advance().content);
		return val ? Value(*val) : Value();
	}
	case TokenType::StringLiteral:
		return Value(std::string(advance().content));
	case TokenType::Boolean:
		return Value(advance().content == "true");
	case TokenType::LBrace:
		return parseMap();
		break;
	case TokenType::LBracket:
		return parseList();
		break;
	case TokenType::EndOfFile:
		break;
	default:
		reportError(peek(), std::string("Unexpected token ") + std::string(peek().content));
		advance();
		break;
	}
	return Value{};
}

const matt::parser::Token& matt::parser::Parser::peek() const
{
	if (isAtEnd())
		return mTokens[mCurrent - 1];

	return mTokens[mCurrent];
}

const matt::parser::Token& matt::parser::Parser::previous() const
{
	return mTokens[mCurrent - 1];
}

const matt::parser::Token& matt::parser::Parser::advance()
{
	if (!isAtEnd())
		++mCurrent;
	return previous();
}

bool matt::parser::Parser::isAtEnd() const
{
	return mCurrent >= mTokens.size();
}

bool matt::parser::Parser::expect(TokenType type, std::string_view errorMessage)
{
	const auto& curr = peek();
	if (curr.type == type)
	{
		advance();
		return true;
	}
	reportError(curr, errorMessage);
	return false;
}

bool matt::parser::Parser::check(TokenType type) const
{
	return peek().type == type;
}

matt::parser::Value matt::parser::Parser::parseMap()
{
	expect(TokenType::LBrace, "Expected '{' to start the map block");

	Value node(Value::Map{});
	while (!isAtEnd() && !check(TokenType::RBrace))
	{
		if (peek().type == TokenType::Identifier)
		{
			std::string key(advance().content);
			if (check(TokenType::LBrace))
				node[key] = parseMap();
			else if (check(TokenType::LBracket))
				node[key] = parseList();
			else
			{
				if (expect(TokenType::Colon, "Expected ':' after key"))
					node[key] = parseValue();
			}
		}
		else
		{
			if (peek().type == TokenType::Comma)
				advance();
			else
			{
				reportError(peek(), std::string("Unexpected token ") + std::string(peek().content));
				advance();
			}
		}
	}
	expect(TokenType::RBrace, "Expected '}' to end the map block");

	return node;
}

matt::parser::Value matt::parser::Parser::parseList()
{
	std::vector<Value> list;
	expect(TokenType::LBracket, "Expected '[' at start of list");

	while (!isAtEnd() && !check(TokenType::RBracket))
	{
		list.push_back(parseValue());
		if (peek().type == TokenType::Comma)
			advance();
	}
	expect(TokenType::RBracket, "Expected ']' at end of list");

	return Value(std::move(list));
}

void matt::parser::Parser::reportError(const Token& token, std::string_view errMessage)
{
	MATT_ERROR(errMessage, "at line:", token.location.line, "at column:", token.location.column);
}

