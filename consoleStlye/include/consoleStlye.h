#pragma once

#include <iostream>
#include <string>

namespace console_stlye
{
    std::string setConsoleStyle(const std::string &text, const std::string &style);
    std::string makeBold(const std::string &text);
    std::string makeUnderlined(const std::string &text);
    std::string makeItalic(const std::string &text);
    std::string makeInverse(const std::string &text);
}