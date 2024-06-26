#include "Color.hpp"


namespace COLOR {
    std::string _as(const std::string &text, const std::string &color) {
        return color + text + RESET;
    }

    std::string as_red(const std::string &text) {
        return _as(text, RED);
    }
}
