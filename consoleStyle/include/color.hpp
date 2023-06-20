// #include <cpp-terminal/terminal.h>
#include <functional>
#include <string>
#include <iostream>

namespace color
{
    template <typename T>
    std::string setColor(T const &value)
    {
        return "\033[" + std::to_string(static_cast<unsigned int>(value)) + 'm';
    }

    enum class Style
    {
        RESET = 0,
        BOLD = 1,
        DIM = 2,
        ITALIC = 3,
        UNDERLINE = 4,
        BLINK = 5,
        BLINK_RAPID = 6,
        REVERSED = 7,
        CONCEAL = 8,
        CROSSED = 9,
        OVERLINE = 53
    };

    enum class ForeGround
    {
        BLACK = 30,
        RED = 31,
        GREEN = 32,
        YELLOW = 33,
        BLUE = 34,
        MAGENTA = 35,
        CYAN = 36,
        WHITE = 37,
        RESET = 39,
        GRAY = 90,
        BRIGHT_RED = 91,
        BRIGHT_GREEN = 92,
        BRIGHT_YELLOW = 93,
        BRIGHT_BLUE = 94,
        BRIGHT_MAGENTA = 95,
        BRIGHT_CYAN = 96,
        BRIGHT_WHITE = 97
    };

    enum class BackGround
    {
        BLACK = 40,
        RED = 41,
        GREEN = 42,
        YELLOW = 43,
        BLUE = 44,
        MAGENTA = 45,
        CYAN = 46,
        WHITE = 47,
        RESET = 49,
        GRAY = 100,
        BRIGHT_RED = 101,
        BRIGHT_GREEN = 102,
        BRIGHT_YELLOW = 103,
        BRIGHT_BLUE = 104,
        BRIGHT_MAGENTA = 105,
        BRIGHT_CYAN = 106,
        BRIGHT_WHITE = 107
    };
} // namespace autocrypt