#include "Lexer.hpp"

#include <iostream>
#include <sstream>

/***********************
*   class LexerError   *
***********************/

LexerError::LexerError(std::string msg)
    : err(std::move(msg))
{}

const char *LexerError::what() const noexcept {
    return err.c_str();
}


/******************
*   class Lexer   *
******************/

Token Lexer::get_token()
{
    char c;
    Pos start;

    bool in_comment = false;
    do {
        start = current;
        c = current.read_char();

        if(std::cin.eof())
            return {start, TokenType::TOK_EOF};

        if(c == '/') {
            c = current.read_char();
            if(c != '/')
                throw_error("Slash '/' must be followed by another slash.");

            in_comment = true;
        }
        else if(c == '\n') {
            in_comment = false;
        }
    } while(isspace(c) || in_comment);

    if(c == '(')
        return {start, TokenType::TOK_BRACE_L};
    if(c == ')')
        return {start, TokenType::TOK_BRACE_R};
    if(c == '[')
        return {start, TokenType::TOK_SQUARE_BRACE_L};
    if(c == ']')
        return {start, TokenType::TOK_SQUARE_BRACE_R};
    if(c == ';')
        return {start, TokenType::TOK_SEMICOLON};
    if(c == ',')
        return {start, TokenType::TOK_COMMA};
    if(c == '+')
        return {start, TokenType::TOK_PLUS};
    if(c == '-')
        return {start, TokenType::TOK_MINUS};
    if(c == '*')
        return {start, TokenType::TOK_STAR};
    if(c == '=')
        return {start, TokenType::TOK_EQUAL};

    if(c == ':') {
        auto n = std::cin.peek();
        if(std::cin.eof() || n != '=')
            return {start, TokenType::TOK_COLON};

        current.read_char();
        return {start, TokenType::TOK_ASSIGN};
    }

    if(c == '>') {
        auto n = std::cin.peek();
        if(std::cin.eof() || n != '=')
            return {start, TokenType::TOK_GREATER};

        current.read_char();
        return {start, TokenType::TOK_GREATER_EQUAL};
    }

    if(c == '.') {
        auto n = std::cin.peek();
        if(std::cin.eof())
            return {start, TokenType::TOK_DOT};

        if(n == '.') {
            current.read_char();
            return {start, TokenType::TOK_DOT_DOT};
        }
        if(std::isdigit(n))
            return parse_real(start, 10, 0, true);

        return {start, TokenType::TOK_DOT};
    }

    if(c == '<') {
        auto n = std::cin.peek();
        if(std::cin.eof())
            return {start, TokenType::TOK_LESS};

        if(n == '=') {
            current.read_char();
            return {start, TokenType::TOK_LESS_EQUAL};
        }
        if(n == '>') {
            current.read_char();
            return {start, TokenType::TOK_NOT_EQUAL};
        }
        return {start, TokenType::TOK_LESS};
    }

    if(c == '&')
        return parse_number(start, 8);
    if(std::isdigit(c))
        return parse_number(start, 10, char_to_digit(c));
    if(c == '$')
        return parse_number(start, 16);

    if(c == '\'')
        return parse_string(start);

    if(is_identifier_char(c, true))
        return parse_identifier_kw(start, c);

    std::stringstream err_msg;
    err_msg << "Invalid character '"
            << c << "'=0x" << std::hex << c
            << ".";

    throw_error(err_msg.str());
}

uint8_t Lexer::parse_digit(uint8_t base) {
    char n = static_cast<char>(std::cin.peek());

    if(std::cin.eof())
        return DIGIT_NO;

    if((base == 8 && is_oct_digit(n)) ||
       (base == 10 && std::isdigit(n)) ||
       (base == 16 && std::isxdigit(n))) {
        current.read_char();
        return char_to_digit(n);
    }

    if(n == '.') {
        current.read_char();
        return DIGIT_DOT;
    }

    if((base <= 10 && (n == 'e' || n == 'E')) ||
       (base == 16 && (n == 'p' || n == 'P'))) {
        current.read_char();
        return DIGIT_EXP;
    }

    if(is_identifier_char(n))
        return DIGIT_IDENT;
    return DIGIT_NO;
}

Token Lexer::parse_number(Pos start, uint8_t base, Mila_int_T number) {
    if(base == 8 || base == 16) {
        uint8_t digit = parse_digit(base);

        if(digit == DIGIT_DOT)
            return parse_real(start, base, 0, false);

        if(digit >= 16) {
            if(base == 8)
                throw_error("'&' must be followed by octal number.");
            throw_error("'$' must be followed by hexadecimal number.");
        }
        number = digit;
    }

    while(true) {
        uint8_t digit = parse_digit(base);
        if(digit == 0 && number == 0)
            throw_error("Number literal cannot start by more zeros.");

        if(digit == DIGIT_NO)
            return {start, number};

        if(digit == DIGIT_IDENT)
            throw_error("Number can not be followed by identifier character.");

        if(digit == DIGIT_DOT)
            return parse_real(start, base, static_cast<double>(number), true);

        if(digit == DIGIT_EXP)
            return parse_exponent(start, base, static_cast<double>(number));

        number = number * base + digit;
    }
}

Token Lexer::parse_real(Pos start, uint8_t base, Mila_real_T number, bool contains_digit) {
    double position_mult = 1;
    while(true) {
        uint8_t digit = parse_digit(base);
        if(digit == DIGIT_NO)
            return {start, number, base};

        if(digit == DIGIT_IDENT)
            throw_error("Number can not be followed by identifier symbol.");

        if(digit == DIGIT_DOT)
            throw_error("Number can not contain two decimal points.");

        if(digit == DIGIT_EXP) {
            if(!contains_digit)
                throw_error("Floating point number literal must contain number in fraction part.");
            return parse_exponent(start, base, number);
        }

        contains_digit = true;
        position_mult /= base;
        number += position_mult * digit;
    }
}

Token Lexer::parse_exponent(Pos start, uint8_t base, Mila_real_T number) {
    Mila_int_T sign = 1;
    Mila_int_T exp = 0;

    auto c = std::cin.peek();

    if(!std::cin.eof() && (c == '-' || c == '+')) {
        current.read_char();
        sign = (c == '-' ? -1 : 1);
    }

    bool nothing = true;
    while(true) {
        uint8_t digit = parse_digit(10);

        if(digit == DIGIT_NO) {
            if(nothing)
                throw_error("E|e|P|p in number literal must be followed by exponent.");

            if(base == 16)
                base = 2;
            return {start, number, base, sign * exp};
        }

        if(digit == DIGIT_IDENT)
            throw_error("Number literal can not be followed by identifier symbol.");

        if(digit == DIGIT_DOT)
            throw_error("Exponent can not be floating point number.");

        if(digit == DIGIT_EXP)
            throw_error("Can not contain two exponents in literal.");

        nothing = false;
        exp = 10 * exp + digit;
    }
}

Token Lexer::parse_string(Pos start) {
    std::vector<char> value;

    bool escaped = false;

    while(true) {
        char c = current.read_char();

        if(std::cin.eof()) {
            std::stringstream ss;
            ss << "String starting at line " << start.line()
               << " position " << start.offset()
               << " was never ended.";
            throw_error(ss.str());
        }

        if(escaped) {
            escaped = false;
            if(c == 'a')       value.push_back('\a');
            else if(c == 'b')  value.push_back('\b');
            else if(c == 'f')  value.push_back('\f');
            else if(c == 'n')  value.push_back('\n');
            else if(c == 'r')  value.push_back('\r');
            else if(c == 't')  value.push_back('\t');
            else if(c == 'v')  value.push_back('\v');
            else if(c == '\\') value.push_back('\\');
            else if(c == '\'') value.push_back('\'');
            else if(c == '"')  value.push_back('"');
            else if(c == '?')  value.push_back('?');
            else if(c == '\n');  // escaping new line
            else if(c == 'x') value.push_back(static_cast<char>(parse_byte_num(16)));
            else if(is_oct_digit(c))
                value.push_back(static_cast<char>(parse_byte_num(8, char_to_digit(c))));
            else {
                warn("Non-standard escape sequence \\" + std::string{c});
                value.push_back('\\');
                value.push_back(c);
            }
        }
        else if(c == '\\') {
            escaped = true;
        }
        else if(c == '\'')
            return {start,
                    Mila_string_T{value.begin(), value.end()},
                    TokenType::TOK_LITERAL_STRING};
        else
            value.push_back(c);
    }
}

Token Lexer::parse_identifier_kw(Pos start, char c) {
    Mila_string_T word = parse_word(c);

    if(word == "begin")
        return {start, TokenType::TOK_BEGIN};
    if(word == "end")
        return {start, TokenType::TOK_END};
    if(word == "const")
        return {start, TokenType::TOK_CONST};
    if(word == "procedure")
        return {start, TokenType::TOK_PROCEDURE};
    if(word == "forward")
        return {start, TokenType::TOK_FORWARD};
    if(word == "function")
        return {start, TokenType::TOK_FUNCTION};
    if(word == "if")
        return {start, TokenType::TOK_IF};
    if(word == "then")
        return {start, TokenType::TOK_THEN};
    if(word == "else")
        return {start, TokenType::TOK_ELSE};
    if(word == "program")
        return {start, TokenType::TOK_PROGRAM};
    if(word == "while")
        return {start, TokenType::TOK_WHILE};
    if(word == "exit")
        return {start, TokenType::TOK_EXIT};
    if(word == "var")
        return {start, TokenType::TOK_VAR};
    if(word == "integer")
        return {start, TokenType::TOK_INTEGER};
    if(word == "real")
        return {start, TokenType::TOK_REAL};
    if(word == "string")
        return {start, TokenType::TOK_STRING};
    if(word == "for")
        return {start, TokenType::TOK_FOR};
    if(word == "do")
        return {start, TokenType::TOK_DO};
    if(word == "of")
        return {start, TokenType::TOK_OF};
    if(word == "to")
        return {start, TokenType::TOK_TO};
    if(word == "downto")
        return {start, TokenType::TOK_DOWNTO};
    if(word == "array")
        return {start, TokenType::TOK_ARRAY};
    if(word == "continue")
        return {start, TokenType::TOK_CONTINUE};
    if(word == "break")
        return {start, TokenType::TOK_BREAK};
    if(word == "or")
        return {start, TokenType::TOK_OR};
    if(word == "mod")
        return {start, TokenType::TOK_MOD};
    if(word == "div")
        return {start, TokenType::TOK_DIV};
    if(word == "not")
        return {start, TokenType::TOK_NOT};
    if(word == "and")
        return {start, TokenType::TOK_AND};
    if(word == "xor")
        return {start, TokenType::TOK_XOR};
    return {start, word, TokenType::TOK_IDENTIFIER};
}

Mila_string_T Lexer::parse_word(char c) {
    std::vector<char> word(1, c);

    while(true) {
        c = static_cast<char>(std::cin.peek());

        if(std::cin.eof() || !is_identifier_char(c))
            return {word.begin(), word.end()};

        current.read_char();
        word.push_back(c);
    }
}

uint8_t Lexer::parse_byte_num(uint8_t base, uint8_t value) {
    if(base == 16) {
        char c = current.read_char();

        if(std::cin.eof() || !std::isxdigit(c))
            throw_error("String escape \\x must be followed by hexadecimal number.");
        value = char_to_digit(c);
    }

    for(size_t i = 0; i < (base == 16 ? 1 : 2); i++) {
        char c = static_cast<char>(std::cin.peek());

        if(std::cin.eof() ||
           (base == 8 && !is_oct_digit(c)) ||
           (base == 16 && !std::isxdigit(c)))
            break;

        current.read_char();
        value <<= base == 16 ? 4 : 3;
        value |= char_to_digit(c);
    }

    return value;
}


[[nodiscard]] bool Lexer::is_identifier_char(char c, bool first) {
    return std::isalpha(c) || c == '_' || (!first && std::isdigit(c));
}

[[nodiscard]] bool Lexer::is_oct_digit(char c) {
    return '0' <= c && c <= '7';
}

[[nodiscard]] uint8_t Lexer::char_to_digit(char c) {
    if('a' <= c && c <= 'f')
        return c - 'a' + 10;
    if('A' <= c && c <= 'F')
        return c - 'A' + 10;
    return c - '0';
}

[[noreturn]] void Lexer::throw_error(const std::string &value) const {
    throw LexerError(value + " " + current.to_string());
}

void Lexer::warn(const std::string &value) const {
    std::cerr << value << " " << current.to_string() << std::endl;
}

