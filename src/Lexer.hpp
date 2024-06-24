#pragma once

#include "Token.hpp"


struct LexerError: std::exception {
    explicit LexerError(std::string msg);

    [[nodiscard]] const char * what() const noexcept override;
private:
    std::string err;
};

struct Lexer {
    [[nodiscard]] Token get_token();

private:
    const static uint8_t DIGIT_NO = -1;
    const static uint8_t DIGIT_IDENT = -2;
    const static uint8_t DIGIT_DOT = -3;
    const static uint8_t DIGIT_EXP = -4;

    [[nodiscard]] Token parse_number(Pos start, uint8_t base, Mila_int_T number = 0);
    [[nodiscard]] uint8_t parse_digit(uint8_t base);
    [[nodiscard]] Token parse_real(Pos start, uint8_t base, Mila_real_T number, bool contains_digit);
    [[nodiscard]] Token parse_exponent(Pos start, uint8_t base, Mila_real_T number);
    [[nodiscard]] Token parse_string(Pos start);
    [[nodiscard]] Token parse_identifier_kw(Pos start, char c);
    [[nodiscard]] Mila_string_T parse_word(char c);
    [[nodiscard]] uint8_t parse_byte_num(uint8_t base, uint8_t value = 0);

    [[nodiscard]] static bool is_identifier_char(char c, bool first = false);

    [[nodiscard]] static bool is_oct_digit(char c);

    [[nodiscard]] static uint8_t char_to_digit(char c);
    [[noreturn]] void throw_error(const std::string &value) const;

    void warn(const std::string &value) const;

    Pos current{1, 0};
};

/* Valid numbers:
 * 0
 * &0  oct
 * $0  hex
 * 1240
 * &476  oct
 * $4aa  hex
 *
 * 0.48154      // real
 * 0.454e455    // exponent
 * 0.445e+415   // plus exp
 * 0.498e-465   // minus exp
 * 1.e8         // exponent after dot
 * .1e8         // first dot
 * 1.000e8      // more zeros after dot
 * 10e65        // no dot(.)
 * $0.5         // hex or oct real in any form
 * $beeepffff   // hex exponent
 *
 * Invalid numbers:
 * 00           // more zeros
 * $00          // more zeros hex or oct
 * .            // only dot
 * .e145        // no number both before and after dot
 * 165e         // no number after e
 */

