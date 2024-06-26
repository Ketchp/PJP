#include "Token.hpp"

#include <iostream>
#include <sstream>
#include <cmath>


/****************
*   class Pos   *
****************/

char Pos::read_char() {
    char c = static_cast<char>(std::cin.get());
    if(!std::cin.eof())
        new_char();
    if(c == '\n')
        new_line();
    return c;
}

[[nodiscard]] int& Pos::line() { return first; }
[[nodiscard]] const int& Pos::line() const { return first; }
[[nodiscard]] int& Pos::offset() { return second; }
[[nodiscard]] const int& Pos::offset() const { return second; }

std::string Pos::to_string() const {
    std::stringstream ss;
    ss << "Line " << first << " position " << second << ".";
    return ss.str();
}

void Pos::new_char() {
    offset()++;
}

void Pos::new_line() {
    line()++;
    offset() = 0;
}

std::string to_string(const TokenType &value) {
    if(value == TokenType::TOK_BEGIN)
        return "begin";
    if(value == TokenType::TOK_END)
        return "end";
    if(value == TokenType::TOK_CONST)
        return "const";
    if(value == TokenType::TOK_PROCEDURE)
        return "procedure";
    if(value == TokenType::TOK_FORWARD)
        return "forward";
    if(value == TokenType::TOK_FUNCTION)
        return "function";
    if(value == TokenType::TOK_IF)
        return "if";
    if(value == TokenType::TOK_THEN)
        return "then";
    if(value == TokenType::TOK_ELSE)
        return "else";
    if(value == TokenType::TOK_PROGRAM)
        return "program";
    if(value == TokenType::TOK_WHILE)
        return "while";
    if(value == TokenType::TOK_EXIT)
        return "exit";
    if(value == TokenType::TOK_VAR)
        return "var";
    if(value == TokenType::TOK_INTEGER)
        return "integer";
    if(value == TokenType::TOK_REAL)
        return "real";
    if(value == TokenType::TOK_STRING)
        return "string";
    if(value == TokenType::TOK_FOR)
        return "for";
    if(value == TokenType::TOK_DO)
        return "do";
    if(value == TokenType::TOK_OF)
        return "of";
    if(value == TokenType::TOK_TO)
        return "to";
    if(value == TokenType::TOK_DOWNTO)
        return "downto";
    if(value == TokenType::TOK_ARRAY)
        return "array";
    if(value == TokenType::TOK_CONTINUE)
        return "continue";
    if(value == TokenType::TOK_BREAK)
        return "break";
    if(value == TokenType::TOK_BRACE_L)
        return "(";
    if(value == TokenType::TOK_BRACE_R)
        return ")";
    if(value == TokenType::TOK_SQUARE_BRACE_L)
        return "[";
    if(value == TokenType::TOK_SQUARE_BRACE_R)
        return "]";
    if(value == TokenType::TOK_SEMICOLON)
        return ";";
    if(value == TokenType::TOK_COMMA)
        return ",";
    if(value == TokenType::TOK_COLON)
        return ":";
    if(value == TokenType::TOK_DOT)
        return ".";
    if(value == TokenType::TOK_MINUS)
        return "-";
    if(value == TokenType::TOK_PLUS)
        return "+";
    if(value == TokenType::TOK_STAR)
        return "*";
    if(value == TokenType::TOK_EQUAL)
        return "=";
    if(value == TokenType::TOK_LESS)
        return "<";
    if(value == TokenType::TOK_GREATER)
        return ">";
    if(value == TokenType::TOK_NOT_EQUAL)
        return "<>";
    if(value == TokenType::TOK_LESS_EQUAL)
        return "<=";
    if(value == TokenType::TOK_GREATER_EQUAL)
        return ">=";
    if(value == TokenType::TOK_ASSIGN)
        return ":=";
    if(value == TokenType::TOK_OR)
        return "or";
    if(value == TokenType::TOK_DOT_DOT)
        return "..";
    if(value == TokenType::TOK_MOD)
        return "mod";
    if(value == TokenType::TOK_DIV)
        return "div";
    if(value == TokenType::TOK_NOT)
        return "not";
    if(value == TokenType::TOK_AND)
        return "and";
    if(value == TokenType::TOK_XOR)
        return "xor";

    if(value == TokenType::TOK_EOF)
        return "EOF";

    if(value == TokenType::TOK_IDENTIFIER)
        return "identifier";

    if(value == TokenType::TOK_LITERAL_INT)
        return "literal integer";
    if(value == TokenType::TOK_LITERAL_REAL)
        return "literal real";
    if(value == TokenType::TOK_LITERAL_STRING)
        return "literal string";
    return "---";
}

std::ostream &operator<<(std::ostream &os, const std::set<TokenType> &values) {
    if(!values.empty()) {
        auto last = --values.end();
        for(auto it = values.begin(); it != last; it++)
            os << *it << ", ";
        os << *last;
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, const TokenType &value) {
    return os << to_string(value);
}


/******************
*   class Token   *
******************/

Token::Token()
    : position{0, 0}, type(TokenType::TOK_EOF)
{}

Token::Token(Pos p, TokenType t)
    : position(std::move(p)), type(t)
{}

Token::Token(Pos p, Mila_int_T number)
    : position(std::move(p)), type(TokenType::TOK_LITERAL_INT), data(number)
{}

Token::Token(Pos p, Mila_real_T fraction, uint8_t base, Mila_int_T exponent)
    : position(std::move(p)),
      type(TokenType::TOK_LITERAL_REAL),
      data(fraction * std::pow(static_cast<Mila_real_T>(base), exponent))
{}

Token::Token(Pos p, Mila_string_T identifier, TokenType t)
    : position(std::move(p)),
      type(is_identifier_or_str_literal(t)),
      data(std::move(identifier))
{}

[[nodiscard]] bool Token::eof() const {
    return type == TokenType::TOK_EOF;
}

[[nodiscard]] size_t Token::size() const {
    return to_string(*this).size();
}

[[nodiscard]] Token Token::NO_MATCH() {
    return Token{{}, TokenType::NO_MATCH};
}

[[nodiscard]] Token::operator bool () const noexcept {
    return type != TokenType::NO_MATCH;
}

[[nodiscard]] TokenType Token::is_identifier_or_str_literal(TokenType t) {
    switch (t) {
        case TokenType::TOK_LITERAL_STRING:
        case TokenType::TOK_IDENTIFIER:
            return t;
        default:
            throw std::exception{};
    }
}

std::string to_string(const Token &token) {
    if(token.type == TokenType::TOK_IDENTIFIER)
        return std::get<Mila_string_T>(token.data);

    if(token.type == TokenType::TOK_LITERAL_INT)
        return std::to_string(std::get<Mila_int_T>(token.data));
    if(token.type == TokenType::TOK_LITERAL_REAL)
        return std::to_string(std::get<Mila_real_T>(token.data));
    if(token.type == TokenType::TOK_LITERAL_STRING)
        return '\'' + std::get<Mila_string_T>(token.data) + '\'';
    return to_string(token.type);
}

std::ostream &operator<<(std::ostream &os, const Token &token) {
    if(token.type == TokenType::TOK_IDENTIFIER)
        return os << '{' << std::get<Mila_string_T>(token.data) << '}';
    return os << to_string(token);
}
