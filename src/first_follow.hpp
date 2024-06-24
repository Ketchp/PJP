#pragma once

#include "Token.hpp"


constexpr auto literal = {
        TokenType::TOK_LITERAL_INT,
        TokenType::TOK_LITERAL_REAL,
        TokenType::TOK_LITERAL_STRING,
};


constexpr auto literal_or_identifier = {
        TokenType::TOK_LITERAL_INT,
        TokenType::TOK_LITERAL_REAL,
        TokenType::TOK_LITERAL_STRING,
        TokenType::TOK_IDENTIFIER,
};

namespace first {
    constexpr auto type = {
            TokenType::TOK_INTEGER,
            TokenType::TOK_REAL,
            TokenType::TOK_STRING,
            TokenType::TOK_ARRAY,
    };

    constexpr auto trivial_type = {
            TokenType::TOK_INTEGER,
            TokenType::TOK_REAL,
            TokenType::TOK_STRING,
    };

    constexpr auto Statement = {
            TokenType::TOK_WHILE,
            TokenType::TOK_IF,
            TokenType::TOK_FOR,
            TokenType::TOK_PLUS,
            TokenType::TOK_MINUS,
            TokenType::TOK_NOT,
            TokenType::TOK_BRACE_L,
            TokenType::TOK_IDENTIFIER,
            TokenType::TOK_LITERAL_INT,
            TokenType::TOK_LITERAL_REAL,
            TokenType::TOK_LITERAL_STRING,
            TokenType::TOK_EXIT,
            TokenType::TOK_CONTINUE,
            TokenType::TOK_BREAK,
    };

    constexpr auto Expression = {
            TokenType::TOK_PLUS,
            TokenType::TOK_MINUS,
            TokenType::TOK_NOT,
            TokenType::TOK_BRACE_L,
            TokenType::TOK_IDENTIFIER,
            TokenType::TOK_LITERAL_INT,
            TokenType::TOK_LITERAL_REAL,
            TokenType::TOK_LITERAL_STRING,
    };

    constexpr auto L6_ = {
            TokenType::TOK_EQUAL,
            TokenType::TOK_NOT_EQUAL,
    };

    constexpr auto L5_ = {
            TokenType::TOK_LESS,
            TokenType::TOK_LESS_EQUAL,
            TokenType::TOK_GREATER,
            TokenType::TOK_GREATER_EQUAL,
    };

    constexpr auto L4_ = {
            TokenType::TOK_PLUS,
            TokenType::TOK_MINUS,
    };

    constexpr auto L3_ = {
            TokenType::TOK_STAR,
            TokenType::TOK_DIV,
            TokenType::TOK_MOD,
    };

    constexpr auto L2_parenthesis = {
            TokenType::TOK_PLUS,
            TokenType::TOK_MINUS,
            TokenType::TOK_NOT,
            TokenType::TOK_BRACE_L,
            TokenType::TOK_IDENTIFIER,
            TokenType::TOK_LITERAL_INT,
            TokenType::TOK_LITERAL_REAL,
            TokenType::TOK_LITERAL_STRING,
    };
}


namespace follow {
    constexpr auto L9_ = {
            TokenType::TOK_SEMICOLON,
            TokenType::TOK_END,
            TokenType::TOK_DO,
            TokenType::TOK_THEN,
            TokenType::TOK_TO,
            TokenType::TOK_DOWNTO,
            TokenType::TOK_BRACE_R,
            TokenType::TOK_SQUARE_BRACE_R,
            TokenType::TOK_ELSE,
            TokenType::TOK_COMMA,
    };

    constexpr auto L8_ = {
            TokenType::TOK_ASSIGN,
            TokenType::TOK_SEMICOLON,
            TokenType::TOK_END,
            TokenType::TOK_DO,
            TokenType::TOK_THEN,
            TokenType::TOK_TO,
            TokenType::TOK_DOWNTO,
            TokenType::TOK_BRACE_R,
            TokenType::TOK_SQUARE_BRACE_R,
            TokenType::TOK_ELSE,
            TokenType::TOK_COMMA,
    };

    constexpr auto L7_ = {
            TokenType::TOK_OR,
            TokenType::TOK_ASSIGN,
            TokenType::TOK_SEMICOLON,
            TokenType::TOK_END,
            TokenType::TOK_DO,
            TokenType::TOK_THEN,
            TokenType::TOK_TO,
            TokenType::TOK_DOWNTO,
            TokenType::TOK_BRACE_R,
            TokenType::TOK_SQUARE_BRACE_R,
            TokenType::TOK_ELSE,
            TokenType::TOK_COMMA,
    };

    constexpr auto L6_ = {
            TokenType::TOK_AND,
            TokenType::TOK_OR,
            TokenType::TOK_ASSIGN,
            TokenType::TOK_SEMICOLON,
            TokenType::TOK_END,
            TokenType::TOK_DO,
            TokenType::TOK_THEN,
            TokenType::TOK_TO,
            TokenType::TOK_DOWNTO,
            TokenType::TOK_BRACE_R,
            TokenType::TOK_SQUARE_BRACE_R,
            TokenType::TOK_ELSE,
            TokenType::TOK_COMMA,
    };

    constexpr auto L5_ = {
            TokenType::TOK_EQUAL,
            TokenType::TOK_NOT_EQUAL,
            TokenType::TOK_AND,
            TokenType::TOK_OR,
            TokenType::TOK_ASSIGN,
            TokenType::TOK_SEMICOLON,
            TokenType::TOK_END,
            TokenType::TOK_DO,
            TokenType::TOK_THEN,
            TokenType::TOK_TO,
            TokenType::TOK_DOWNTO,
            TokenType::TOK_BRACE_R,
            TokenType::TOK_SQUARE_BRACE_R,
            TokenType::TOK_ELSE,
            TokenType::TOK_COMMA,
    };

    constexpr auto L4_ = {
            TokenType::TOK_LESS,
            TokenType::TOK_GREATER,
            TokenType::TOK_LESS_EQUAL,
            TokenType::TOK_GREATER_EQUAL,
            TokenType::TOK_EQUAL,
            TokenType::TOK_NOT_EQUAL,
            TokenType::TOK_AND,
            TokenType::TOK_OR,
            TokenType::TOK_ASSIGN,
            TokenType::TOK_SEMICOLON,
            TokenType::TOK_END,
            TokenType::TOK_DO,
            TokenType::TOK_THEN,
            TokenType::TOK_TO,
            TokenType::TOK_DOWNTO,
            TokenType::TOK_BRACE_R,
            TokenType::TOK_SQUARE_BRACE_R,
            TokenType::TOK_ELSE,
            TokenType::TOK_COMMA,
    };

    constexpr auto L3_ = {
            TokenType::TOK_PLUS,
            TokenType::TOK_MINUS,
            TokenType::TOK_LESS,
            TokenType::TOK_GREATER,
            TokenType::TOK_LESS_EQUAL,
            TokenType::TOK_GREATER_EQUAL,
            TokenType::TOK_EQUAL,
            TokenType::TOK_NOT_EQUAL,
            TokenType::TOK_AND,
            TokenType::TOK_OR,
            TokenType::TOK_ASSIGN,
            TokenType::TOK_SEMICOLON,
            TokenType::TOK_END,
            TokenType::TOK_DO,
            TokenType::TOK_THEN,
            TokenType::TOK_TO,
            TokenType::TOK_DOWNTO,
            TokenType::TOK_BRACE_R,
            TokenType::TOK_SQUARE_BRACE_R,
            TokenType::TOK_ELSE,
            TokenType::TOK_COMMA,
    };

    constexpr auto L1_ = {
            TokenType::TOK_STAR,
            TokenType::TOK_DIV,
            TokenType::TOK_MOD,
            TokenType::TOK_PLUS,
            TokenType::TOK_MINUS,
            TokenType::TOK_LESS,
            TokenType::TOK_GREATER,
            TokenType::TOK_LESS_EQUAL,
            TokenType::TOK_GREATER_EQUAL,
            TokenType::TOK_EQUAL,
            TokenType::TOK_NOT_EQUAL,
            TokenType::TOK_AND,
            TokenType::TOK_OR,
            TokenType::TOK_ASSIGN,
            TokenType::TOK_SEMICOLON,
            TokenType::TOK_END,
            TokenType::TOK_DO,
            TokenType::TOK_THEN,
            TokenType::TOK_TO,
            TokenType::TOK_DOWNTO,
            TokenType::TOK_BRACE_R,
            TokenType::TOK_SQUARE_BRACE_R,
            TokenType::TOK_ELSE,
            TokenType::TOK_COMMA,
    };

}





