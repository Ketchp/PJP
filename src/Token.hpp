#pragma once

#include <tuple>
#include <string>
#include <vector>
#include <limits>
#include <variant>
#include <cstdint>
#include <exception>


using Mila_int_T = int32_t;
using Mila_real_T = double;
using Mila_string_T = std::string;

using Mila_variant_T = std::variant<std::monostate,
                                    Mila_int_T,
                                    Mila_real_T,
                                    Mila_string_T>;


constexpr bool Mila_int_T_signed = std::is_signed<Mila_int_T>();
constexpr unsigned Mila_int_T_bits = sizeof(Mila_int_T) * 8;

struct Pos : public std::pair<int, int> {
    using std::pair<int, int>::pair;

    char read_char();

    [[nodiscard]] int& line();
    [[nodiscard]] const int& line() const;
    [[nodiscard]] int& offset();
    [[nodiscard]] const int& offset() const;

    [[nodiscard]] std::string to_string() const;

private:
    void new_char();
    void new_line();
};


enum class TokenType {
    NO_MATCH,  // special initial_value used for indication of non-matched token

    TOK_EOF,

    // numbers and identifiers
    TOK_IDENTIFIER,
    TOK_LITERAL_INT,            // decimal $(hex) &(oct)
    TOK_LITERAL_REAL,           // float
    TOK_LITERAL_STRING,         // 'string'

    // keywords
    TOK_BEGIN,                 // begin
    TOK_END,                   // end
    TOK_CONST,                 // const
    TOK_PROCEDURE,             // procedure
    TOK_FORWARD,               // forward
    TOK_FUNCTION,              // function
    TOK_IF,                    // if
    TOK_THEN,                  // then
    TOK_ELSE,                  // else
    TOK_PROGRAM,               // program
    TOK_WHILE,                 // while
    TOK_EXIT,                  // exit
    TOK_VAR,                   // var
    TOK_INTEGER,               // integer
    TOK_REAL,                  // real
    TOK_STRING,                // string
    TOK_FOR,                   // for
    TOK_DO,                    // do
    TOK_OF,                    // of
    TOK_TO,                    // to
    TOK_DOWNTO,                // downto
    TOK_ARRAY,                 // array

    // braces
    TOK_BRACE_L,               // (
    TOK_BRACE_R,               // )
    TOK_SQUARE_BRACE_L,        // [
    TOK_SQUARE_BRACE_R,        // ]

    // 1-character symbols
    TOK_SEMICOLON,             // ;
    TOK_COMMA,                 // ,
    TOK_COLON,                 // :
    TOK_DOT,                   // .
    TOK_MINUS,                 // -
    TOK_PLUS,                  // +
    TOK_STAR,                  // *
    TOK_EQUAL,                 // =
    TOK_LESS,                  // <
    TOK_GREATER,               // >

    // 2-character operators
    TOK_NOT_EQUAL,             //  <>
    TOK_LESS_EQUAL,            //  <=
    TOK_GREATER_EQUAL,         //  >=
    TOK_ASSIGN,                //  :=
    TOK_OR,                    //  or
    TOK_DOT_DOT,               //  ..

    // 3-character operators (keywords)
    TOK_MOD,                   // mod
    TOK_DIV,                   // div
    TOK_NOT,                   // not
    TOK_AND,                   // and
    TOK_XOR,                   // xor
};

/*
 * Lexer returns tokens [0-255] if it is an unknown character, otherwise one of these for known things.
 * Here are all valid tokens:
 */
struct Token {
    Token();
    Token(Pos p, TokenType t);
    Token(Pos p, Mila_int_T number);
    Token(Pos p, Mila_real_T fraction, uint8_t base, Mila_int_T exponent = 0);
    Token(Pos p, Mila_string_T identifier, TokenType t);

    [[nodiscard]] bool eof() const;

    [[nodiscard]] static Token NO_MATCH();
    [[nodiscard]] explicit operator bool() const noexcept;

    Pos position;
    TokenType type;
    Mila_variant_T data;
private:
    [[nodiscard]] static TokenType is_identifier_or_str_literal(TokenType t);
};

std::ostream &operator<<(std::ostream &os, const Token &token);
