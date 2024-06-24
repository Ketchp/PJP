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

std::ostream &operator<<(std::ostream &os, const Token &token) {
    if(token.type == TokenType::TOK_EOF)
        return os << "EOF";
    if(token.type == TokenType::TOK_IDENTIFIER)
        return os << '{' << std::get<Mila_string_T>(token.data) << '}';
    if(token.type == TokenType::TOK_LITERAL_INT)
        return os << std::get<Mila_int_T>(token.data);
    if(token.type == TokenType::TOK_LITERAL_REAL)
        return os << std::get<Mila_real_T>(token.data);
    if(token.type == TokenType::TOK_LITERAL_STRING)
        return os << '\'' << std::get<Mila_string_T>(token.data) << '\'';
    if(token.type == TokenType::TOK_BEGIN)
        return os << "begin";
    if(token.type == TokenType::TOK_END)
        return os << "end";
    if(token.type == TokenType::TOK_CONST)
        return os << "const";
    if(token.type == TokenType::TOK_PROCEDURE)
        return os << "procedure";
    if(token.type == TokenType::TOK_FORWARD)
        return os << "forward";
    if(token.type == TokenType::TOK_FUNCTION)
        return os << "function";
    if(token.type == TokenType::TOK_IF)
        return os << "if";
    if(token.type == TokenType::TOK_THEN)
        return os << "then";
    if(token.type == TokenType::TOK_ELSE)
        return os << "else";
    if(token.type == TokenType::TOK_PROGRAM)
        return os << "program";
    if(token.type == TokenType::TOK_WHILE)
        return os << "while";
    if(token.type == TokenType::TOK_EXIT)
        return os << "exit";
    if(token.type == TokenType::TOK_VAR)
        return os << "var";
    if(token.type == TokenType::TOK_INTEGER)
        return os << "integer";
    if(token.type == TokenType::TOK_REAL)
        return os << "real";
    if(token.type == TokenType::TOK_STRING)
        return os << "string";
    if(token.type == TokenType::TOK_FOR)
        return os << "for";
    if(token.type == TokenType::TOK_DO)
        return os << "do";
    if(token.type == TokenType::TOK_OF)
        return os << "of";
    if(token.type == TokenType::TOK_TO)
        return os << "to";
    if(token.type == TokenType::TOK_DOWNTO)
        return os << "downto";
    if(token.type == TokenType::TOK_ARRAY)
        return os << "array";
    if(token.type == TokenType::TOK_CONTINUE)
        return os << "continue";
    if(token.type == TokenType::TOK_BREAK)
        return os << "break";
    if(token.type == TokenType::TOK_BRACE_L)
        return os << "(";
    if(token.type == TokenType::TOK_BRACE_R)
        return os << ")";
    if(token.type == TokenType::TOK_SQUARE_BRACE_L)
        return os << "[";
    if(token.type == TokenType::TOK_SQUARE_BRACE_R)
        return os << "]";
    if(token.type == TokenType::TOK_SEMICOLON)
        return os << ";";
    if(token.type == TokenType::TOK_COMMA)
        return os << ",";
    if(token.type == TokenType::TOK_COLON)
        return os << ":";
    if(token.type == TokenType::TOK_DOT)
        return os << ".";
    if(token.type == TokenType::TOK_MINUS)
        return os << "-";
    if(token.type == TokenType::TOK_PLUS)
        return os << "+";
    if(token.type == TokenType::TOK_STAR)
        return os << "*";
    if(token.type == TokenType::TOK_EQUAL)
        return os << "=";
    if(token.type == TokenType::TOK_LESS)
        return os << "<";
    if(token.type == TokenType::TOK_GREATER)
        return os << ">";
    if(token.type == TokenType::TOK_NOT_EQUAL)
        return os << "<>";
    if(token.type == TokenType::TOK_LESS_EQUAL)
        return os << "<=";
    if(token.type == TokenType::TOK_GREATER_EQUAL)
        return os << ">=";
    if(token.type == TokenType::TOK_ASSIGN)
        return os << ":=";
    if(token.type == TokenType::TOK_OR)
        return os << "or";
    if(token.type == TokenType::TOK_DOT_DOT)
        return os << "..";
    if(token.type == TokenType::TOK_MOD)
        return os << "mod";
    if(token.type == TokenType::TOK_DIV)
        return os << "div";
    if(token.type == TokenType::TOK_NOT)
        return os << "not";
    if(token.type == TokenType::TOK_AND)
        return os << "and";
    if(token.type == TokenType::TOK_XOR)
        return os << "xor";
    throw std::exception{};
}
