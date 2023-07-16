#include "console_style.hpp"

namespace console_stlye
{
    std::string setConsoleStyle(const std::string &text, const std::string &style)
    {
        return "\033[" + style + "m" + text + "\033[0m";
    }

    std::string makeBold(const std::string &text)
    {
        return setConsoleStyle(text, "1");
    }

    std::string makeUnderlined(const std::string &text)
    {
        return setConsoleStyle(text, "4");
    }

    std::string makeItalic(const std::string &text)
    {
        return setConsoleStyle(text, "3");
    }

    std::string makeInverse(const std::string &text)
    {
        return setConsoleStyle(text, "7");
    }
}