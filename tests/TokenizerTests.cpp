#include <gtest/gtest.h>
#include <array>
#include <span>
#include "matt/parser/Tokenizer.h"

namespace 
{
    using Type = matt::parser::TokenType;
    struct ExpectedPair
    {
        Type type;
        std::string_view content;
    };
}

void AssertToken(const matt::parser::Token& t, matt::parser::TokenType type, std::string_view content)
{
	EXPECT_EQ(t.type, type);
	EXPECT_EQ(t.content, content);
}

void testTokenizer(std::span<const ExpectedPair> expected, std::string_view content)
{
    matt::parser::Tokenizer tokenizer;
    auto tokens = tokenizer.tokenize(content);
    ASSERT_EQ(tokens.size(), expected.size()) << "Tokens count mismatch!";

    for (size_t i = 0; i < tokens.size(); ++i)
    {
        SCOPED_TRACE("Failure at token index: " + std::to_string(i));
        AssertToken(tokens[i], expected[i].type, expected[i].content);
    }
}

TEST(TokenizerTest, BasicSyntax)
{
    std::string_view content = R"(
        # Global Config
        version: 1.2
        author: "Matt"
        enabled: true
        data {
            points: [-10, 0, 10.5]
        }
    )";
    constexpr auto expected = std::to_array<ExpectedPair>({
         { Type::Identifier,    "version"     },
         { Type::Colon,         ":"           },
         { Type::NumberDouble,  "1.2"         },
         { Type::Identifier,    "author"      },
         { Type::Colon,         ":"           },
         { Type::StringLiteral, "Matt"        },
         { Type::Identifier,    "enabled"     },
         { Type::Colon,         ":"           },
         { Type::Boolean,       "true"        },
         { Type::Identifier,    "data"        },
         { Type::LBrace,        "{"           },
         { Type::Identifier,    "points"      },
         { Type::Colon,         ":"           },
         { Type::LBracket,      "["           },
         { Type::NumberInt,     "-10"         },
         { Type::Comma,         ","           }, 
         { Type::NumberInt,     "0"           },
         { Type::Comma,         ","           }, 
         { Type::NumberDouble,  "10.5"        },
         { Type::RBracket,      "]"           },
         { Type::RBrace,        "}"           },
         { Type::EndOfFile,     "End Of File" }  
        });

    testTokenizer(expected, content);
}


TEST(TokenizerTest, NestedComplexStructures)
{

    std::string_view content = R"(
        player {
            max_speed: -10.5
            tags: ["hero", "fast"]
            pos { x: 100 }
        }
    )";
    constexpr auto expected = std::to_array<ExpectedPair>({
        { Type::Identifier,    "player"     },
        { Type::LBrace,        "{"          },
        { Type::Identifier,    "max_speed"  },
        { Type::Colon,         ":"          },
        { Type::NumberDouble,  "-10.5"      },
        { Type::Identifier,    "tags"       },
        { Type::Colon,         ":"          },
        { Type::LBracket,      "["          },
        { Type::StringLiteral, "hero"       },
        { Type::Comma,         ","          },
        { Type::StringLiteral, "fast"       },
        { Type::RBracket,      "]"          },
        { Type::Identifier,    "pos"        },
        { Type::LBrace,        "{"          },
        { Type::Identifier,    "x"          },
        { Type::Colon,         ":"          },
        { Type::NumberInt,     "100"        },
        { Type::RBrace,        "}"          },
        { Type::RBrace,        "}"          },
        { Type::EndOfFile,     "End Of File"}
        });

    testTokenizer(expected, content);
}

TEST(TokenizerTest, NoWhitespace)
{
    std::string_view content = "pos{x:10 y:-5 z:20.0}tags:[\"a\",1,true]";

    constexpr auto expected = std::to_array<ExpectedPair>({
        { Type::Identifier,    "pos"   },
        { Type::LBrace,        "{"     },
        { Type::Identifier,    "x"     },
        { Type::Colon,         ":"     },
        { Type::NumberInt,     "10"    },
        { Type::Identifier,    "y"     },
        { Type::Colon,         ":"     },
        { Type::NumberInt,     "-5"    },
        { Type::Identifier,    "z"     },
        { Type::Colon,         ":"     },
        { Type::NumberDouble,  "20.0"  },
        { Type::RBrace,        "}"     },
        { Type::Identifier,    "tags"  },
        { Type::Colon,         ":"     },
        { Type::LBracket,      "["     },
        { Type::StringLiteral, "a"     },
        { Type::Comma,         ","     },
        { Type::NumberInt,     "1"     },
        { Type::Comma,         ","     },
        { Type::Boolean,       "true"  },
        { Type::RBracket,      "]"     },
        { Type::EndOfFile,     "End Of File"}
        });

    testTokenizer(expected, content);
}