#include "Tokenizer.h"

matt::parser::Tokenizer::Tokenizer()
	:mLine(1),
	mColumn(1),
	mCurrent(0),
	mStart(0)
{
}

matt::parser::TokenVector matt::parser::Tokenizer::tokenize(std::string_view content)
{
	mContent = content;
	mCurrent = 0;
	mLine = 1;
	mColumn = 1;
	mTokens.clear();

	while (!isAtEnd())
	{
		mStart = mCurrent;
		auto c = advance();
		scanToken(c);
	}
	mTokens.emplace_back(TokenType::EndOfFile, "End Of File", TokenLocation{ mLine, mColumn });
	return mTokens;
}

bool matt::parser::Tokenizer::isAtEnd() const
{
	return mCurrent >= mContent.size();
}

char matt::parser::Tokenizer::peek() const
{
	if (isAtEnd())
		return NullChar;

	return mContent[mCurrent];
}

char matt::parser::Tokenizer::peekNext() const
{
	if (mCurrent + 1 >= mContent.size())
		return NullChar;

	return mContent[mCurrent + 1];
}

char matt::parser::Tokenizer::advance() 
{
	char c = mContent[mCurrent];
	++mCurrent;
	++mColumn;
	return c;
}

void matt::parser::Tokenizer::scanToken(char c)
{
	TokenLocation tokenLoc{ mLine, mColumn - 1 };
	switch (c)
	{
	case '\n':
		++mLine;
		mColumn = 1;
		break;
	case ' ':
	case '\r':
	case '\t':
		break;
	case '#':
		skipComment();
		break;
	case '{':
		addToken(TokenType::LBrace, tokenLoc);
		break;
	case '}':
		addToken(TokenType::RBrace, tokenLoc);
		break;
	case '[':
		addToken(TokenType::LBracket, tokenLoc);
		break;
	case ']':
		addToken(TokenType::RBracket, tokenLoc);
		break;
	case ':':
		addToken(TokenType::Colon, tokenLoc);
		break;
	case ',':
		addToken(TokenType::Comma, tokenLoc);
		break;
	case '"':
		readString(tokenLoc);
		break;
	case '-':
		if (isdigit(peek()))
			readNumber(tokenLoc);
		else
			mTokens.emplace_back(TokenType::Unknown, "Unknown symbol", tokenLoc);
		break;
	default:
		if (isdigit(c))
			readNumber(tokenLoc);
		else if (isalpha(c) || c == '_') // '_' for variables like '_Name'; 'max_hp' or something else.
			readIdentifier(tokenLoc);
		else
			mTokens.emplace_back(TokenType::Unknown, "Unknown symbol", tokenLoc);
		break;
	}
}

void matt::parser::Tokenizer::skipComment()
{
	while (peek() != '\n' && !isAtEnd())
		advance();
}

void matt::parser::Tokenizer::readString(const TokenLocation& loc)
{
	while (peek() != '"')
	{
		if (peek() == '\n')
		{
			++mLine;
			mColumn = 1;
		}
		if (isAtEnd()) 
		{
			mTokens.emplace_back(TokenType::Unknown, "Unterminated string", loc);
			return;
		}
		advance();
	}
	advance();
	int length = static_cast<int>(mCurrent) - static_cast<int>(mStart);
	auto text = (length >= 2) ? mContent.substr(mStart + 1, mCurrent - mStart - 2) : "";
	mTokens.emplace_back(TokenType::StringLiteral, text, loc);
}

void matt::parser::Tokenizer::readNumber(const TokenLocation& loc)
{
	bool decimal = false;
	while (isdigit(peek()) || peek() == '.')
	{
		char c = peek();
		if (c == '.')
		{
			if (decimal)
			{
				mTokens.emplace_back(TokenType::Unknown, "Invalid number literal", loc);
				while (isdigit(peek()) || peek() == '.')
					advance();
				return;
			}
			decimal = true;
		}
		advance();
	}
	auto type = decimal ? TokenType::NumberDouble : TokenType::NumberInt;
	addToken(type, loc);
}

void matt::parser::Tokenizer::readIdentifier(const TokenLocation& loc)
{
	//Using isalnum because the identifier might have numbers in it
	while (isalnum(peek()) || peek() == '_')
		
		advance();

	auto text = mContent.substr(mStart, mCurrent - mStart);
	auto type = TokenType::Identifier;
	if (text == "true" || text == "false")
		type = TokenType::Boolean;
	mTokens.emplace_back(type, text, loc);
}

void matt::parser::Tokenizer::addToken(TokenType type, const TokenLocation& loc)
{
	auto text = mContent.substr(mStart, mCurrent - mStart);
	mTokens.push_back({ type, text, loc });
}


