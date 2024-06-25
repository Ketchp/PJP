#pragma once
#include <iostream>
#include <string>

#include "Token.hpp"


struct LexerError: std::exception {
    explicit LexerError(std::string msg);

    [[nodiscard]] const char * what() const noexcept override;
private:
    std::string err;
};

enum class CharType {
    NEUTRAL,
    NUMBER,
    STRING,
    KEY_OR_ID,
    KEYWORD,
    IDENTIFIER,
    COMMENT,
};

constexpr std::string ESC_S{"\x1B["};
constexpr std::string COL_RESET{"0m"};
constexpr std::string COL_BLUE{"38;5;32m"};     // number
constexpr std::string COL_GREEN{"38;5;83m"};    // string
constexpr std::string COL_ORANGE{"38;5;214m"};  // keyword
constexpr std::string COL_LIGHT{"38;5;229m"};   // identifier
constexpr std::string COL_GREY{"38;5;59m"};     // comment
constexpr std::string COL_RED{"38;5;196m"};      // error

struct Lexer {
    [[nodiscard]] Token get_token();
    std::ostream &print_highlighted(std::ostream &os) const;
private:
    const static uint8_t DIGIT_NO = -1;
    const static uint8_t DIGIT_IDENT = -2;
    const static uint8_t DIGIT_DOT = -3;
    const static uint8_t DIGIT_EXP = -4;

    [[nodiscard]] Token parse_number(Pos start, uint8_t base, Mila_int_T number = 0);
    [[nodiscard]] Token parse_number_col(Pos start, uint8_t base, Mila_int_T number = 0);
    [[nodiscard]] uint8_t parse_digit(uint8_t base);
    [[nodiscard]] Token parse_real(Pos start, uint8_t base, Mila_real_T number, bool contains_digit);
    [[nodiscard]] Token parse_real_col(Pos start, uint8_t base, Mila_real_T number, bool contains_digit);
    [[nodiscard]] Token parse_exponent(Pos start, uint8_t base, Mila_real_T number);
    [[nodiscard]] Token parse_string(Pos start);
    [[nodiscard]] Token parse_string_col(Pos start);
    [[nodiscard]] Token parse_identifier_kw(Pos start, char c);
    [[nodiscard]] Token parse_identifier_kw_col(Pos start, char c);
    [[nodiscard]] Mila_string_T parse_word(char c);
    [[nodiscard]] uint8_t parse_byte_num(uint8_t base, uint8_t value = 0);

    [[nodiscard]] static bool is_identifier_char(char c, bool first = false);

    [[nodiscard]] static bool is_oct_digit(char c);

    [[nodiscard]] static uint8_t char_to_digit(char c);

    void warn(const std::string &value) const;
    [[noreturn]] void error(const std::string &message);
    [[noreturn]] static void throw_error(Pos err_pos, const std::string &value = "");

    char read_char();

    void reset_type();
    void set_type(CharType new_type);

    Pos position{1, 0};
    std::vector<std::string> pretty_lines{{""}};
    std::string word_buffer;
    CharType col_type;
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

