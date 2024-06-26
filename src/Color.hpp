#pragma once

#include <string>

#define ESCAPE_START "\x1B["

namespace COLOR {
    constexpr std::string BLUE   = ESCAPE_START "38;5;32m";
    constexpr std::string GREEN  = ESCAPE_START "38;5;83m";
    constexpr std::string ORANGE = ESCAPE_START "38;5;214m";
    constexpr std::string LIGHT  = ESCAPE_START "38;5;229m";
    constexpr std::string GREY   = ESCAPE_START "38;5;59m";
    constexpr std::string RED    = ESCAPE_START "38;5;196m";
    constexpr std::string RESET  = ESCAPE_START "0m";

    std::string _as(const std::string &text, const std::string &color);
    std::string as_red(const std::string &text);
}
